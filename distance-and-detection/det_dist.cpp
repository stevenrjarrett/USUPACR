// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <string>


#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>

#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/cudaFont.h>

#include <jetson-inference/detectNet.h>

// Signal handler, makes it so that we don't interrupt tensor-flow at a bad time?

double distance3d(cv::point3d pt1, cv::point3d pt2)
{
    return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.y-pt2.y,2) + pow(pt1.z-pt2.z,2))
}

cv::Point3d getCentroid(const cv::Mat &depthMat, rs2::depth_frame& dframe, const cv::Rect& drect)
{
    // get units information
    double units = dframe.get_units();
    // get center coordinates in the frame
    double frame_x_index = drect.x + drect.width/2;
    double frame_y_index = drect.y + drect.height/2;
    double z = dframe.get_distance(frame_x_index, frame_y_index);

    // calculate x angle
    double frame_x = frame_x_index - (depthMat.cols/2);
    double xangle = atan(frame_x/((double)depthMat.cols/2)*tan(IR_Hor_Field_of_View/2));//x/((double)lframe.cols/2) * 30.0;
    // calculate y angle
    double frame_y = frame_y_index - (depthMat.rows/2);
    double yangle = atan(frame_y/((double)depthMat.rows/2)*tan(IR_Ver_Field_of_View/2));//x/((double)lframe.cols/2) * 30.0;

    double x = dist * tan(xangle);
    double y = dist * tan(yangle);


    return cv::Point3d(x, y, z);
}

bool exit_signal_recieved = false;

void sig_handler(int signo)
{
	if( signo == SIGINT )
	{
		printf("received SIGINT\n");
		exit_signal_recieved = true;
	}
}

// point handlers for color and depth bounding boxes
int max_fps = 30;

double IR_Hor_Field_of_View = 91.2*M_PI/180;
double IR_Ver_Field_of_View = 65.5*M_PI/180;
double IR_width  = 1280;
double IR_height = 720;
int IR_numPixels = IR_height * IR_width;
double COL_Hor_Field_of_View = 69.4*M_PI/180;
double COL_Ver_Field_of_View = 42.5*M_PI/180;
double COL_width  = 1280;
double COL_height = 720;
int COL_numPixels = COL_height * COL_width;

double x_color_to_depth_conversion_factor = IR_width /COL_width  * tan(COL_Hor_Field_of_View/2)/tan(IR_Hor_Field_of_View/2);
double y_color_to_depth_conversion_factor = IR_height/COL_height * tan(COL_Ver_Field_of_View/2)/tan(IR_Ver_Field_of_View/2);

#define CVT_COLOR_TO_DEPTH 0
#define CVT_DEPTH_TO_COLOR 1

cv::Rect2d cvt_bb(cv::Rect2d bb, int cvt_type)
{
    int x      = (int)bb.x;
    int y      = (int)bb.y;
    int width  = (int)bb.width;
    int height = (int)bb.height;
    switch(cvt_type)
    {
        case CVT_COLOR_TO_DEPTH:
            x      = static_cast<int>((((int)bb.x - COL_width /2) * x_color_to_depth_conversion_factor) + IR_width /2);
            y      = static_cast<int>((((int)bb.y - COL_height/2) * y_color_to_depth_conversion_factor) + IR_height/2);
            width  = static_cast<int>((int)bb.width  * x_color_to_depth_conversion_factor);
            height = static_cast<int>((int)bb.height * y_color_to_depth_conversion_factor);
            break;
        case CVT_DEPTH_TO_COLOR:
            x      = static_cast<int>((((int)bb.x - IR_width /2) / x_color_to_depth_conversion_factor) + COL_width /2);
            y      = static_cast<int>((((int)bb.y - IR_height/2) / y_color_to_depth_conversion_factor) + COL_height/2);
            width  = static_cast<int>((int)bb.width  / x_color_to_depth_conversion_factor);
            height = static_cast<int>((int)bb.height / y_color_to_depth_conversion_factor);
            break;
    }
    return cv::Rect2d(x, y, width, height);
}

//struct bb_color
//{
//    int x1=0;
//    int y1=0;
//    int x2=0;
//    int y2=0;
//    int width=0;
//    int height=0;
//
//    bb_color(int _x1, int _y1, int _x2, int _y2)
//      : x1(_x1),
//        y1(_y1),
//        x2(_x2),
//        y2(_y2),
//        width(_x2-_x1),
//        height(_y2-_y1)
//    {
//        correctVals();
//    }
//
//    void correctVals()
//    {
//        if(width<0)
//        {
//            std::swap(x1, x2);
//            width *= -1;
//        }
//        if(height<0)
//        {
//            std::swap(y1, y2);
//            height *= -1;
//        }
//    }
//
//    void setFromDepth(bb_depth bbd)
//    {
//
//    }
//};


int main(int argc, char * argv[]) try
{
    // Set up camera streams and realsense

        // Declare depth colorizer for pretty visualization of depth data
        rs2::colorizer color_map;

        // Set up resolution and other variables
//        int COL_width = COL_width;
//        int color_height = COL_height;
//        int depth_width = IR_width;
//        int IR_height = IR_height;




        //Create a configuration for configuring the pipeline with a non default profile
        rs2::config cfg;
        //Add desired streams to configuration
        cfg.enable_stream(RS2_STREAM_COLOR   , COL_width, COL_height, RS2_FORMAT_RGB8, max_fps);
        cfg.enable_stream(RS2_STREAM_INFRARED, IR_width, IR_height, RS2_FORMAT_Y8  , max_fps);
        cfg.enable_stream(RS2_STREAM_DEPTH   , IR_width, IR_height, RS2_FORMAT_Z16 , max_fps);

        // enable signal catcher
        if( signal(SIGINT, sig_handler) == SIG_ERR )
            printf("\ncan't catch SIGINT\n");

        // Declare RealSense pipeline, encapsulating the actual device and sensors
        rs2::pipeline pipe;
        // Start streaming with default recommended configuration
        pipe.start(cfg);

        const auto depth_window_name = "Depth Video Feed";
        const auto color_window_name = "Color Video Feed";
        cv::namedWindow(depth_window_name, cv::WINDOW_AUTOSIZE);
        cv::namedWindow(color_window_name, cv::WINDOW_AUTOSIZE);




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
            printf("detecnet:  failed to alloc image memory\n");
            return 0;
        }

        /*
         * create detectNet
         */
        detectNet* net = detectNet::Create();

        if( !net )
        {
            printf("detectnet-camera:   failed to initialize imageNet\n");
            return 0;
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

        if( !cudaAllocMapped((void**)&bbCPU, (void**)&bbCUDA, maxBoxes * sizeof(float4)) ||
            !cudaAllocMapped((void**)&confCPU, (void**)&confCUDA, maxBoxes * classes * sizeof(float)) )
        {
            printf("detectnet-console:  failed to alloc output memory\n");
            return 0;
        }

        /*
         * processing loop
         */
        float confidence = 0.0f;


	//////////////////////////////////////////////////////////////////////////
	// main loop

    while (cv::waitKey(1) < 0 && cv::getWindowProperty(depth_window_name, cv::WND_PROP_AUTOSIZE) >= 0
                              && cv::getWindowProperty(depth_window_name, cv::WND_PROP_AUTOSIZE) >= 0
                              && !exit_signal_recieved)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera

        // get frames
        rs2::frame depth = data.get_depth_frame();
        rs2::frame color = data.get_color_frame();

        // get data pointers
        char* depthData = (char*)depth.get_data();
        char* colorData = (char*)color.get_data();

//        std::fstream outFile("depthImage.csv", std::fstream::out | std::fstream::trunc);
////        int imgWidth  = camera->GetWidth();
////        int imgHeight = camera->GetHeight();
////        int numPixels = imgWidth*imgHeight;
////        float *fltPtr = (float*)colorData_flt_CPU;
//        for(int i=0; i<IR_width; i++)
//            outFile << (std::string)"\"" + std::to_string(i) + (std::string)"\",";
//        outFile << "\n";
////        std::cout << "Starting copy" << std::endl;
//        for(int i=0; i<COL_numPixels; i++)
//        {
//            if(i%((int)IR_width) == 0)
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
        cv::Mat depthMat(cv::Size(IR_width, IR_height), CV_16UC1, depthData, cv::Mat::AUTO_STEP);

        cv::Mat color_image_raw(cv::Size(COL_width, COL_height), CV_8UC3, colorData, cv::Mat::AUTO_STEP);
        cv::Mat colorMat;
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

//		if( net->Detect((float*)imgRGBA, camera->GetWidth(), camera->GetHeight(), bbCPU, &numBoundingBoxes, confCPU))
		if( net->Detect(colorData_flt_CUDA, rgba_width, rgba_height , bbCPU, &numBoundingBoxes, confCPU))
		{
			printf("%i bounding boxes detected\n", numBoundingBoxes);

			int lastClass = 0;
			int lastStart = 0;

			for( int n=0; n < numBoundingBoxes; n++ )
			{
				const int nc = confCPU[n*2+1];
				float* bb = bbCPU + (n * 4);

                // Collect the bounding box and create the bounding box for the depth camera

//				printf("detected obj %i  class #%u (%s)  confidence=%f\n", n, nc, net->GetClassDesc(nc), confCPU[n*2]);
//				printf("bounding box %i  (%f, %f)  (%f, %f)  w=%f  h=%f\n", n, bb[0], bb[1], bb[2], bb[3], bb[2] - bb[0], bb[3] - bb[1]);
                cv::Rect2d crect(bb[0],bb[1], bb[2]-bb[0], bb[3]-bb[1]);
                cv::Rect2d drect = cvt_bb(crect, CVT_COLOR_TO_DEPTH);

//                printf("bw box       %i  (%f, %f)  w=%f  h=%f\n", n, drect.x, drect.y, drect.width, drect.height);
                cv::rectangle(colorMat, crect, cv::Scalar( 255, 0, 0 ), 2, 1 );
                cv::rectangle(depthMat, drect, cv::Scalar( 255, 255, 255 ), 2, 1 );

                std::string prnt = "Confidence: ";
                prnt += std::to_string(confCPU[n*2]);
                cv::putText(colorMat, prnt, cv::Point(bb[0],bb[1]-10), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50,170,50),2);

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
                cv::putText(colorMat, std::to_string(position.z), cv::Point(bb[0],bb[1]-50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50,170,50),2);


				// Add it to the global list
			}

			/*if( font != NULL )
			{
				char str[256];
				sprintf(str, "%05.2f%% %s", confidence * 100.0f, net->GetClassDesc(img_class));

				font->RenderOverlay((float4*)imgRGBA, (float4*)imgRGBA, camera->GetWidth(), camera->GetHeight(),
								    str, 10, 10, make_float4(255.0f, 255.0f, 255.0f, 255.0f));
			}*/

//			if( display != NULL )
//			{
//				char str[256];
//				sprintf(str, "TensorRT %i.%i.%i | %s | %04.1f FPS", NV_TENSORRT_MAJOR, NV_TENSORRT_MINOR, NV_TENSORRT_PATCH, precisionTypeToStr(net->GetPrecision()), display->GetFPS());
//				display->SetTitle(str);
//			}
//			char c = cv::waitKey(5);
//			if(c==27) // ESC is pressed
//                break;
		}


		// update display


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
//        cv::Mat color_image_raw(cv::Size(COL_width, COL_height), CV_8UC4, colorData_flt_CPU, cv::Mat::AUTO_STEP);
//        cv::Mat colorMat;
//        cv::cvtColor(color_image_raw, colorMat, cv::COLOR_RGBA2BGR);
//        cv::cvtColor(rgbaMat, colorMat, cv::COLOR_RGBA2BGR);

        // Update the window with new data
//        cv::rectangle(depthMat, cv::Rect2d(5, 5, 50, 50), cv::Scalar(.9,.9,.9), 2, 1 );
//        cv::rectangle(depthMat, cv::Rect2d(60, 5, 50, 50), cv::Scalar(0.5,0.5,0.5), 2, 1 );
//        cv::rectangle(depthMat, cv::Rect2d(115, 5, 50, 50), cv::Scalar(0,0,0), 2, 1 );


        cv::Mat dpth;
//        depthMat.convertTo(dpth, CV_32F, 1.0 / 255, 0);
        cv::normalize(depthMat, dpth, 0, 255, cv::NORM_MINMAX);

        imshow(depth_window_name, dpth);
        imshow(color_window_name, colorMat);
//        usleep(500000);
    }

    cudaFree(colorData_flt_CPU);
    cudaFree(colorData_flt_CUDA);
    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
