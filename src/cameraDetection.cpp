#include "cameraDetection.h"

//void emptyFunc(){}

cameraDetection::cameraDetection()
{
    /// Set defaults
    max_fps = 30;


    std::string colorWindowName = "Color Video Feed";
    COL_Hor_Field_of_View = 69.4*M_PI/180;
    COL_Ver_Field_of_View = 42.5*M_PI/180;
//    COL_width  = 1280;
//    COL_height = 720;
    COL_width  = 640;
    COL_height = 480;
    COL_numPixels = COL_height * COL_width;

    std::string depthWindowName = "Depth Video Feed";
    DEPTH_Hor_Field_of_View = 91.2*M_PI/180;
    DEPTH_Ver_Field_of_View = 65.5*M_PI/180;
//    DEPTH_width  = 1280;
//    DEPTH_height = 720;
    DEPTH_width  = 640;
    DEPTH_height = 480;
    DEPTH_numPixels = DEPTH_height * DEPTH_width;
//    depth_scale;

    x_color_to_depth_conversion_factor = DEPTH_width /COL_width  * tan(COL_Hor_Field_of_View/2)/tan(DEPTH_Hor_Field_of_View/2);
    y_color_to_depth_conversion_factor = DEPTH_height/COL_height * tan(COL_Ver_Field_of_View/2)/tan(DEPTH_Ver_Field_of_View/2);


    isRunning  = false;
    show_color = false;
    show_depth = false;
    show_boxes = true;
    this->stop_signal_recieved = false;
    lastTime   = 0;
    // Initialize detectnet
    net = detectNet::Create();
    // initialize thread. It will not run continuously unless isRunning is true.
    runningThread = std::thread(&cameraDetection::run, this);
//    runningThread.join();
}

cameraDetection::~cameraDetection()
{
    this->stop_signal_recieved = true;
    isRunning = false;
//    stop();
    std::cout << "Stopping camera..." << std::endl;
    if(runningThread.joinable())
        runningThread.join();
    std::cout << "\tDone" << std::endl;
    //dtor
}

void cameraDetection::resetCamera()
{
    // Report to the console
    std::cout << "Resetting camera..." << std::endl;
    // Stop the running detection process
    stop();
    // Update pertinant variablesint max_fps = 30;

    DEPTH_numPixels = DEPTH_height * DEPTH_width;
    COL_numPixels = COL_height * COL_width;

    x_color_to_depth_conversion_factor = DEPTH_width /COL_width  * tan(COL_Hor_Field_of_View/2)/tan(DEPTH_Hor_Field_of_View/2);
    y_color_to_depth_conversion_factor = DEPTH_height/COL_height * tan(COL_Ver_Field_of_View/2)/tan(DEPTH_Ver_Field_of_View/2);
    // Wait for 0.1 second to allow the computer to take a breath.
    usleep(100000);
    // Start the process again
    start();

}

void cameraDetection::start()
{
//    if(runningThread.joinable() && !isRunning)
//        stop();
    isRunning = true;
//    if(!runningThread.joinable())
//        runningThread = std::thread(&cameraDetection::run, this);
}

void cameraDetection::stop()
{
    isRunning = false;
//    std::cout << "Stopping camera" << std::endl;
//    if(runningThread.joinable())
//        runningThread.join();
}

void cameraDetection::bboxFix( const cv::Mat& img, cv::Rect2d& box)
{
    if(box.x < 0)
    {
        box.width += box.x;
        box.x = 0;
    }
    if(box.y < 0)
    {
        box.height += box.y;
        box.y = 0;
    }
    if(box.x+box.width >= img.cols)
    {
        box.width = img.cols-1-box.x;
    }
    if(box.y+box.height >= img.rows)
    {
        box.height = img.rows-1-box.y;
    }
}

cv::Rect2d cameraDetection::cvt_bb(cv::Rect2d bb, int cvt_type)
{
    int x      = (int)bb.x;
    int y      = (int)bb.y;
    int width  = (int)bb.width;
    int height = (int)bb.height;
    switch(cvt_type)
    {
        case CVT_COLOR_TO_DEPTH:
            x      = static_cast<int>((((int)bb.x - COL_width /2) * x_color_to_depth_conversion_factor) + DEPTH_width /2);
            y      = static_cast<int>((((int)bb.y - COL_height/2) * y_color_to_depth_conversion_factor) + DEPTH_height/2);
            width  = static_cast<int>((int)bb.width  * x_color_to_depth_conversion_factor);
            height = static_cast<int>((int)bb.height * y_color_to_depth_conversion_factor);
            break;
        case CVT_DEPTH_TO_COLOR:
            x      = static_cast<int>((((int)bb.x - DEPTH_width /2) / x_color_to_depth_conversion_factor) + COL_width /2);
            y      = static_cast<int>((((int)bb.y - DEPTH_height/2) / y_color_to_depth_conversion_factor) + COL_height/2);
            width  = static_cast<int>((int)bb.width  / x_color_to_depth_conversion_factor);
            height = static_cast<int>((int)bb.height / y_color_to_depth_conversion_factor);
            break;
    }
    return cv::Rect2d(x, y, width, height);
}

cv::Point3d cameraDetection::getCentroid(cv::Mat &depthMat, const rs2::depth_frame& dframe, cv::Rect drect)
{
    // get units information
//    double units = dframe.get_units();
    // get center coordinates in the frame
    double frame_x_index = drect.x + drect.width/2;
    double frame_y_index = drect.y + drect.height/2;

    // Get z distance
    cv::Rect2d innerRect(drect.x + drect.width  / 3,
                         drect.y + drect.height / 3,
                         drect.width  / 3,
                         drect.height / 3);
    bboxFix(depthMat, innerRect);
    cv::rectangle(depthMat, innerRect, DEPTH_TEXT_COLOR, 2, 1 );
    const cv::Mat innerMat = depthMat(innerRect);
    cv::Scalar tmpScal = cv::mean(innerMat);
    double z = tmpScal[0] * depth_scale;

//    double z = depthMat.at((int)frame_x_index, (int)frame_y_index) * depth_scale;

    // calculate x angle
    double frame_x = frame_x_index - (depthMat.cols/2);
    double xangle = atan(frame_x/((double)depthMat.cols/2)*tan(DEPTH_Hor_Field_of_View/2));//x/((double)lframe.cols/2) * 30.0;
    // calculate y angle
    double frame_y = frame_y_index - (depthMat.rows/2);
    double yangle = atan(frame_y/((double)depthMat.rows/2)*tan(DEPTH_Ver_Field_of_View/2));//x/((double)lframe.cols/2) * 30.0;

    double x = z * tan(xangle);
    double y = z * tan(yangle);


    return cv::Point3d(x, y, z);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////       Main       /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void cameraDetection::run() try
{
while(!(this->stop_signal_recieved))
{
    // don't even try if I shouldn't be running
    while(!isRunning && !(this->stop_signal_recieved))
        usleep(1000);
    if(this->stop_signal_recieved)
        break;
    // Set up camera streams and realsense

        // Declare depth colorizer for pretty visualization of depth data
//        rs2::colorizer color_map;

        // Set up resolution and other variables
//        int COL_width = COL_width;
//        int color_height = COL_height;
//        int depth_width = DEPTH_width;
//        int DEPTH_height = DEPTH_height;




        //Create a configuration for configuring the pipeline with a non default profile
        rs2::config cfg;
        //Add desDEPTHed streams to configuration
        cfg.enable_stream(RS2_STREAM_COLOR   , COL_width, COL_height, RS2_FORMAT_RGB8, max_fps);
//        cfg.enable_stream(RS2_STREAM_INFRARED, DEPTH_width, DEPTH_height, RS2_FORMAT_Y8  , max_fps);
        cfg.enable_stream(RS2_STREAM_DEPTH   , DEPTH_width, DEPTH_height, RS2_FORMAT_Z16 , max_fps);
//        cfg.rs435_depth_emitter_enabled = 1;

        // Declare RealSense pipeline, encapsulating the actual device and sensors
        rs2::pipeline pipe;
        // Start streaming with default recommended configuration
        rs2::pipeline_profile selection = pipe.start(cfg);

        // enable emitter
        rs2::device selected_device = selection.get_device();
        auto depth_sensor = selected_device.first<rs2::depth_sensor>();

        if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
        {
            depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1.f); // Enable emitter
//            depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 0.f); // Disable emitter
        }
        if (depth_sensor.supports(RS2_OPTION_LASER_POWER))
        {
            // Query min and max values:
            auto range = depth_sensor.get_option_range(RS2_OPTION_LASER_POWER);
            depth_sensor.set_option(RS2_OPTION_LASER_POWER, range.max); // Set max power
//            depth_sensor.set_option(RS2_OPTION_LASER_POWER, 0.f); // Disable laser
        }

        // Get depth scale
        auto sensor = selection.get_device().first<rs2::depth_sensor>();
        depth_scale =  sensor.get_depth_scale() * DISTANCE_CORRECTION;





    ///////////////////////////////////////////////////////////////////
    //Set up detectNet

        float *colorData_flt_CPU,
              *colorData_flt_CUDA;
//        cudaMallocManaged(&colorData_flt, sizeof(float)*COL_numPixels*4);
//        char*  colorDataCPU,
//               colorDataCUDA;
//        cudaMallocManaged(&colorData, 10 * sizeof(char)*COL_numPixels*4);

        if( !cudaAllocMapped((void**)&colorData_flt_CPU, (void**)&colorData_flt_CUDA, COL_numPixels * sizeof(float4)) )
        {
            std::cout << "detectnet:  failed to alloc image memory\n";
            return;
        }

        /*
         * create detectNet
         */
//        detectNet* net = detectNet::Create();

        if( !net )
        {
            std::cout << "detectnet-camera:   failed to initialize imageNet\n";
            return;
        }


        /*
         * allocate memory for output bounding boxes and class confidence
         */
        const uint32_t maxBoxes = net->GetMaxBoundingBoxes();
        const uint32_t classes  = net->GetNumClasses();

        float* bbCPU    = NULL;
        float* bbCUDA   = NULL;
        float* confCPU  = NULL;
        float* confCUDA = NULL;

        if( !cudaAllocMapped((void**)&bbCPU  , (void**)&bbCUDA  , maxBoxes * sizeof(float4)) ||
            !cudaAllocMapped((void**)&confCPU, (void**)&confCUDA, maxBoxes * classes * sizeof(float)) )
        {
            std::cout << "detectnet:  failed to alloc output memory\n";
            return;
        }

        /*
         * processing loop
         */
        float confidence = 0.0f;

    // set up display windows
//        const auto depth_window_name = "Depth Video Feed";
//        const auto color_window_name = "Color Video Feed";
        if(show_color)
            cv::namedWindow(colorWindowName, cv::WINDOW_AUTOSIZE);
        if(show_depth)
            cv::namedWindow(depthWindowName, cv::WINDOW_AUTOSIZE);

	//////////////////////////////////////////////////////////////////////////
	// main loop

    while (isRunning && !(this->stop_signal_recieved))
    {
//        if(cv::waitKey(1) == 27)
//        {
//            isRunning = false;
//            return;
////            stop_signal_recieved = true;
//        }
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera

        // get frames
        rs2::frame depth = data.get_depth_frame();
        rs2::frame color = data.get_color_frame();

        // get data pointers
        unsigned short *depthData = (unsigned short *)depth.get_data();
        char           *colorData = (char*)color.get_data();

//        std::fstream outFile("depthImage.csv", std::fstream::out | std::fstream::trunc);
////        int imgWidth  = camera->GetWidth();
////        int imgHeight = camera->GetHeight();
////        int numPixels = imgWidth*imgHeight;
////        float *fltPtr = (float*)colorData_flt_CPU;
//        for(int i=0; i<DEPTH_width; i++)
//            outFile << (std::string)"\"" + std::to_string(i) + (std::string)"\",";
//        outFile << "\n";
////        std::cout << "Starting copy" << std::endl;
//        for(int i=0; i<COL_numPixels; i++)
//        {
//            if(i%((int)DEPTH_width) == 0)
//                outFile << "\n";
//            int index = i;
////            std::cout << "i     = " << i << " index = " << index << std::endl;
//            outFile << (int)(depthData[index+0]) << ",";
////            std::cout << "i     = " << i << " index = " << index << std::endl;
//        }
//        outFile.close();
//        std::cout << "Copied image successfully" << std::endl;
////        cv::Mat fltImg(rgbimg.size(), CV_32FC4, colorData_flt);
//        usleep(500000);

        // create opencv Mat's
        depthMat = cv::Mat(cv::Size(DEPTH_width, DEPTH_height), CV_16UC1, depthData, cv::Mat::AUTO_STEP);

        cv::Mat color_image_raw(cv::Size(COL_width, COL_height), CV_8UC3, colorData, cv::Mat::AUTO_STEP);
//        cv::Mat colorMat;
//        cv::Mat rgbaMat(cv::Size(COL_width, COL_height), CV_64FC4, colorData_flt_CPU, cv::Mat::AUTO_STEP);
        cv::cvtColor(color_image_raw, colorMat, cv::COLOR_RGB2BGR);
//        cv::cvtColor(color_image_raw, rgbaMat, cv::COLOR_RGB2RGBA);

        //Color

        // Query frame size (width and height)
        const int w2 = color.as<rs2::video_frame>().get_width();
        const int h2 = color.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
//        std::cout << "Loaded frames" << std::endl;






////////////////////////////////////////////////////////////////////////////////////////////////////////



//        cv::cvtColor(rgbimg, continuousRGBA, CV_BGR2RGBA, 4);
        int rgba_width  = COL_width;
        int rgba_height = COL_height;
        for(int i=0; i<COL_numPixels; i++)
        {
            int rgb_ind = i*3;
            int rgba_ind = i*4;
            colorData_flt_CPU[rgba_ind+0] = (float)colorData[rgb_ind+0];
            colorData_flt_CPU[rgba_ind+1] = (float)colorData[rgb_ind+1];
            colorData_flt_CPU[rgba_ind+2] = (float)colorData[rgb_ind+2];
            colorData_flt_CPU[rgba_ind+3] = 255.0;
        }

		// classify image with detectNet
		int numBoundingBoxes = maxBoxes;

		if( net->Detect(colorData_flt_CUDA, rgba_width, rgba_height , bbCPU, &numBoundingBoxes, confCPU))
		{
//			printf("%i bounding boxes detected\n", numBoundingBoxes);

			int lastClass = 0;
			int lastStart = 0;

			// Clear the last vector of people and re-fill.
			lastPeople.clear();

			for( int n=0; n < numBoundingBoxes; n++ )
			{
				const int nc = confCPU[n*2+1];
				float* bb = bbCPU + (n * 4);
                float confidence = confCPU[n*2];
                // Collect the bounding box and create the bounding box for the depth camera

//				printf("detected obj %i  class #%u (%s)  confidence=%f\n", n, nc, net->GetClassDesc(nc), confCPU[n*2]);
//				printf("bounding box %i  (%f, %f)  (%f, %f)  w=%f  h=%f\n", n, bb[0], bb[1], bb[2], bb[3], bb[2] - bb[0], bb[3] - bb[1]);
                cv::Rect2d crect(bb[0],bb[1], bb[2]-bb[0], bb[3]-bb[1]);
                cv::Rect2d drect = cvt_bb(crect, CVT_COLOR_TO_DEPTH);

//                printf("bw box       %i  (%f, %f)  w=%f  h=%f\n", n, drect.x, drect.y, drect.width, drect.height);
                if(show_boxes)
                    cv::rectangle(colorMat, crect, COL_TEXT_COLOR, 2, 1 );
                if(show_boxes)
                    cv::rectangle(depthMat, drect, DEPTH_TEXT_COLOR, 2, 1 );

                if(show_boxes)
                {
                    std::string prnt = "Confidence: ";
                    prnt += std::to_string(confidence);
                    cv::putText(colorMat, prnt, cv::Point(bb[0],bb[1]+20), cv::FONT_HERSHEY_SIMPLEX, 0.75, COL_TEXT_COLOR ,2);
                }

				if( nc != lastClass || n == (numBoundingBoxes - 1) )
				{
                    // draw boxes in the RGBA image
//					if( !net->DrawBoxes(colorData_flt_CUDA, colorData_flt_CUDA, rgba_width, rgba_height,
//						                        bbCUDA + (lastStart * 4), (n - lastStart) + 1, lastClass) )
//						printf("detectnet-console:  failed to draw boxes\n");


					lastClass = nc;
					lastStart = n;

					CUDA(cudaDeviceSynchronize());
				}

				// Get 3d centroid of person
				cv::Point3d position = getCentroid(depthMat, depth, drect);
				if(show_boxes)
                {
                    cv::putText(colorMat, std::string("x =  ") + std::to_string(position.x), cv::Point(bb[0],bb[1]+40), cv::FONT_HERSHEY_SIMPLEX, 0.75, COL_TEXT_COLOR,2);
                    cv::putText(colorMat, std::string("y =  ") + std::to_string(position.y), cv::Point(bb[0],bb[1]+60), cv::FONT_HERSHEY_SIMPLEX, 0.75, COL_TEXT_COLOR,2);
                    cv::putText(colorMat, std::string("z =  ") + std::to_string(position.z), cv::Point(bb[0],bb[1]+80), cv::FONT_HERSHEY_SIMPLEX, 0.75, COL_TEXT_COLOR,2);
                }


				// Add it to the global list
				lastPeople.push_back(personFrame(position, crect, confidence));
			}
            lastTime = watch.abs_time_LongLong();
		}


		// update display


        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        if(show_depth)
        {
//            cv::Mat dpth;
//            depthMat.convertTo(dpth, CV_8UC1, 2.0 / 255, 0);
    //        cv::normalize(depthMat, dpth, 0, 255, cv::NORM_MINMAX);

            imshow(depthWindowName, depthMat);
        }
        if(show_color)
            imshow(colorWindowName, colorMat);
        usleep(1000);
    }

//    std::cout << "Camera: Freeing colorData_flt_CPU" << std::endl;
//    std::cout << "Camera: Freeing colorData_flt_CUDA" << std::endl;
//    std::cout << "Camera: Freeing bbCPU" << std::endl;

    if(show_depth)
    {
        cv::destroyWindow(depthWindowName);
    }
    if(show_color)
        cv::destroyWindow(colorWindowName);

    cudaFree(colorData_flt_CPU);
    cudaFree(colorData_flt_CUDA);
    cudaFree(bbCPU   );
    cudaFree(bbCUDA  );
    cudaFree(confCPU );
    cudaFree(confCUDA);
//    return EXIT_SUCCESS;
}
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return;
}
