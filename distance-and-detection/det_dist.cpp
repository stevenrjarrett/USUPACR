// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/cudaFont.h>

#include <jetson-inference/detectNet.h>

bool exit_signal_recieved = false;

void sig_handler(int signo)
{
	if( signo == SIGINT )
	{
		printf("received SIGINT\n");
		exit_signal_recieved = true;
	}
}



int main(int argc, char * argv[]) try
{
    // Set up camera streams and realsense

        // Declare depth colorizer for pretty visualization of depth data
        rs2::colorizer color_map;

        // Set up resolution and other variables
        int color_width = 640;
        int color_height = 480;
        int depth_width = 1280;
        int depth_height = 720;

        int color_numElements = color_height * color_width;
        int depth_numElements = depth_height * depth_width;

        int max_fps = 30;



        //Create a configuration for configuring the pipeline with a non default profile
        rs2::config cfg;
        //Add desired streams to configuration
        cfg.enable_stream(RS2_STREAM_COLOR   , color_width, color_height, RS2_FORMAT_RGB8, max_fps);
        cfg.enable_stream(RS2_STREAM_INFRARED, depth_width, depth_height, RS2_FORMAT_Y8  , max_fps);
        cfg.enable_stream(RS2_STREAM_DEPTH   , depth_width, depth_height, RS2_FORMAT_Z16 , max_fps);

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
//        cudaMallocManaged(&colorData_flt, sizeof(float)*color_numElements*4);
//        char*  colorDataCPU,
//               colorDataCUDA;
//        cudaMallocManaged(&colorData, 10 * sizeof(char)*color_numElements*4);

        if( !cudaAllocMapped((void**)&colorData_flt_CPU, (void**)&colorData_flt_CUDA, color_numElements * sizeof(float4)) )
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

        // create opencv Mat's
        cv::Mat depthMat(cv::Size(depth_width, depth_height), CV_16UC1, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

        cv::Mat color_image_raw(cv::Size(color_width, color_height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat colorMat;
        cv::cvtColor(color_image_raw, colorMat, cv::COLOR_RGB2BGR);

        //Color

        // Query frame size (width and height)
        const int w2 = color.as<rs2::video_frame>().get_width();
        const int h2 = color.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
//        std::cout << "Loaded frames" << std::endl;






////////////////////////////////////////////////////////////////////////////////////////////////////////




//
//	/*
//	 * create the camera device
//	 */
//    cv::Mat rgbimg;
//    rgbcam >> rgbimg;
//    cv::imshow("Original", rgbimg);
////    cv::waitKey();
//
//    // Allocate memory
//    unsigned long numElem = rgbimg.total()*4;
//    uchar* camData = new uchar[numElem];
//    cv::Mat continuousRGBA(rgbimg.size(), CV_8UC4, camData);
//
//
//    if(!rgbcam.isOpened())
//    {
//		printf("\ndetectnet-camera:  failed to initialize video device\n");
//		return 0;
//    }
//
//	printf("\ndetectnet-camera:  successfully initialized video device\n");
//    std::cout << "Image height: " << rgbcam.get(cv::CAP_PROP_FRAME_HEIGHT) << '\n';
//    std::cout << "Image width:  " << rgbcam.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;



//	while( !exit_signal_recieved )
//	{
//		void* imgCPU  = NULL;
//		void* imgCUDA = NULL;

		// get the latest frame
//		if( !camera->Capture(&imgCPU, &imgCUDA, 1000) )
//			printf("\ndetectnet-camera:  failed to capture frame\n");

//        rgbcam >> rgbimg;
//		if( rgbimg.empty() )
//			printf("\ndetectnet-camera:  failed to capture frame\n");
//        cv::imshow("Original", rgbimg);
//        cv::waitKey();

		// convert from RGB to RGBA and move to graphics memory

//        cv::cvtColor(rgbimg, continuousRGBA, CV_BGR2RGBA, 4);
        int rgba_width  = color_width;
        int rgba_height = color_height;
        for(int i=0; i<color_numElements; i++)
        {
            int rgb_ind = i*3;
            int rgba_ind = i*4;
            colorData_flt_CPU[rgba_ind]   = (float)colorData[rgb_ind];
            colorData_flt_CPU[rgba_ind+1] = (float)colorData[rgb_ind+1];
            colorData_flt_CPU[rgba_ind+2] = (float)colorData[rgb_ind+2];
            colorData_flt_CPU[rgba_ind+3] = 255.0;
        }
//        std::cout << "Copied image successfully" << std::endl;
//        cv::Mat fltImg(rgbimg.size(), CV_32FC4, colorData_flt);


//		void* imgRGBA = NULL;
//
//		if( !camera->ConvertRGBA(imgCUDA, &imgRGBA) )
//			printf("detectnet-camera:  failed to convert from NV12 to RGBA\n");

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

				printf("detected obj %i  class #%u (%s)  confidence=%f\n", n, nc, net->GetClassDesc(nc), confCPU[n*2]);
				printf("bounding box %i  (%f, %f)  (%f, %f)  w=%f  h=%f\n", n, bb[0], bb[1], bb[2], bb[3], bb[2] - bb[0], bb[3] - bb[1]);

				if( nc != lastClass || n == (numBoundingBoxes - 1) )
				{
//					if( !net->DrawBoxes(colorData_flt, colorData_flt, rgba_width, rgba_height,
//						                        bbCUDA + (lastStart * 4), (n - lastStart) + 1, lastClass) )
//						printf("detectnet-console:  failed to draw boxes\n");

                    cv::rectangle(colorMat, cv::Rect2d(bb[0],bb[1], bb[2]-bb[0], bb[3]-bb[1]), cv::Scalar( 255, 0, 0 ), 2, 1 );

                    std::string prnt = "Confidence: ";
                    prnt += std::string(confCPU[n*2]);
                    cv::putText(colorMat, prnt, cv::Point(bb[0],bb[1]), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50,170,50),2);

					lastClass = nc;
					lastStart = n;

					CUDA(cudaDeviceSynchronize());
				}
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
//        cv::Mat img;
//        img.LoadFromPixels(rgbimg.cols, rgbimg.rows, camData);
//        char str[256];
//        sprintf(str, "TensorRT %i.%i.%i | %s | %04.1f FPS", NV_TENSORRT_MAJOR, NV_TENSORRT_MINOR, NV_TENSORRT_PATCH, precisionTypeToStr(net->GetPrecision()), 0.0);
//		cv::imshow(str, fltImg);
//		if( display != NULL )
//		{
//			display->UserEvents();
//			display->BeginRender();
//
//			if( texture != NULL )
//			{
//				// rescale image pixel intensities for display
//				CUDA(cudaNormalizeRGBA((float4*)imgRGBA, make_float2(0.0f, 255.0f),
//								   (float4*)imgRGBA, make_float2(0.0f, 1.0f),
//		 						   camera->GetWidth(), camera->GetHeight()));
//
//				// map from CUDA to openGL using GL interop
//				void* tex_map = texture->MapCUDA();
//
//				if( tex_map != NULL )
//				{
//					cudaMemcpy(tex_map, imgRGBA, texture->GetSize(), cudaMemcpyDeviceToDevice);
//					texture->Unmap();
//				}
//
//				// draw the texture
//				texture->Render(100,100);
//			}
//
//			display->EndRender();
//		}
//	}






        ////////////////////////////////////////////////////////////////////////////////////////////////////////


        // Update the window with new data
        imshow(depth_window_name, depthMat);
        imshow(color_window_name, colorMat);
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
