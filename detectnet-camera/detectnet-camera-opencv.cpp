/*
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// Detectnet program, but adapted to use opencv instead of gstreamer.

// #include <jetson-utils/gstCamera.h>

//#include <jetson-utils/glDisplay.h>
//#include <jetson-utils/glTexture.h>

#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/cudaFont.h>

#include <jetson-inference/detectNet.h>


#define DEFAULT_CAMERA 3	// -1 for onboard camera, or change to index of /dev/video V4L2 camera (>=0)


bool signal_recieved = false;

void sig_handler(int signo)
{
	if( signo == SIGINT )
	{
		printf("received SIGINT\n");
		signal_recieved = true;
	}
}


int main( int argc, char** argv )
{
	printf("detectnet-camera\n  args (%i):  ", argc);

	for( int i=0; i < argc; i++ )
		printf("%i [%s]  ", i, argv[i]);

	printf("\n\n");


	if( signal(SIGINT, sig_handler) == SIG_ERR )
		printf("\ncan't catch SIGINT\n");


	/*
	 * create the camera device
	 */

    cv::VideoCapture rgbcam(DEFAULT_CAMERA);
    int camera_rows = rgbcam.get(cv::CAP_PROP_FRAME_HEIGHT);
    int camera_cols = rgbcam.get(cv::CAP_PROP_FRAME_WIDTH);
    unsigned long numElem = camera_rows * camera_cols;
    uchar* camData = new uchar[numElem];
    float* camDataflt;
    cudaMallocManaged(&camDataflt, sizeof(float)*numElem);
    cv::Mat rgbaImg(camera_rows, camera_cols, CV_8UC4, camData);
    cv::Mat fltImg(camera_rows, camera_cols, CV_32FC4, camDataflt);

    rgbcam >> rgbaImg;
    cv::imshow("Original", rgbaImg);
//    cv::waitKey();

//    uchar* camData = new uchar[rgbimg.total()*4];
//    cv::Mat rgbaImg(rgbimg.size(), CV_8UC4, camData);
//    cv::cvtColor(rgbimg, rgbaImg, CV_BGR2RGBA, 4);
//    img.LoadFromPixels(rgbimg.cols, rgbimg.rows, camData);

    if(!rgbcam.isOpened())
    {
		printf("\ndetectnet-camera:  failed to initialize video device\n");
		return 0;
    }

	printf("\ndetectnet-camera:  successfully initialized video device\n");
    std::cout << "Image height: " << rgbcam.get(cv::CAP_PROP_FRAME_HEIGHT) << '\n';
    std::cout << "Image width:  " << rgbcam.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
//	printf("    width:  %u\n", rgbimg.cols);
//	printf("   height:  %u\n", rgbimg.rows);
//	printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth());

//	gstCamera* camera = gstCamera::Create(DEFAULT_CAMERA);
//
//	if( !camera )
//	{
//		printf("\ndetectnet-camera:  failed to initialize video device\n");
//		return 0;
//	}
//
//	printf("\ndetectnet-camera:  successfully initialized video device\n");
//	printf("    width:  %u\n", camera->GetWidth());
//	printf("   height:  %u\n", camera->GetHeight());
//	printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth());


	/*
	 * create detectNet
	 */
	detectNet* net = detectNet::Create(argc, argv);

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
	 * create openGL window
	 */
//	glDisplay* display = glDisplay::Create();
//	glTexture* texture = NULL;
//
//	if( !display ) {
//		printf("\ndetectnet-camera:  failed to create openGL display\n");
//	}
//	else
//	{
//		texture = glTexture::Create(camera->GetWidth(), camera->GetHeight(), GL_RGBA32F_ARB/*GL_RGBA8*/);
//
//		if( !texture )
//			printf("detectnet-camera:  failed to create openGL texture\n");
//	}

    // Create opencv window:
//    cv::imshow("Original", rgbimg);

	/*
	 * create font
	 */
//	cudaFont* font = cudaFont::Create();


	/*
	 * start streaming
	 */
//	if( !camera->Open() )
//	{
//		printf("\ndetectnet-camera:  failed to open camera for streaming\n");
//		return 0;
//	}
//
//	printf("\ndetectnet-camera:  camera open for streaming\n");


	/*
	 * processing loop
	 */
	float confidence = 0.0f;

	while( !signal_recieved )
	{
		void* imgCPU  = NULL;
		void* imgCUDA = NULL;

		// get the latest frame
//		if( !camera->Capture(&imgCPU, &imgCUDA, 1000) )
//			printf("\ndetectnet-camera:  failed to capture frame\n");

        rgbcam >> rgbaImg;
		if( rgbaImg.empty() )
			printf("\ndetectnet-camera:  failed to capture frame\n");
        cv::imshow("Original", rgbaImg);
//        cv::waitKey();

		// convert from YUV to RGBA and move to graphics memory

//        cv::cvtColor(rgbaImg, rgbaImg, CV_BGR2RGBA, 4);
        for(int i=0; i<numElem; i+=4)
        {
            camDataflt[i]   = (float)camData[i];
            camDataflt[i+1] = (float)camData[i+1];
            camDataflt[i+2] = (float)camData[i+2];
            camDataflt[i+3] = (float)camData[i+3];
        }


//		void* imgRGBA = NULL;
//
//		if( !camera->ConvertRGBA(imgCUDA, &imgRGBA) )
//			printf("detectnet-camera:  failed to convert from NV12 to RGBA\n");

		// classify image with detectNet
		int numBoundingBoxes = maxBoxes;

//		if( net->Detect((float*)imgRGBA, camera->GetWidth(), camera->GetHeight(), bbCPU, &numBoundingBoxes, confCPU))
		if( net->Detect(camDataflt, rgbaImg.cols, rgbaImg.rows , bbCPU, &numBoundingBoxes, confCPU))
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
					if( !net->DrawBoxes(camDataflt, camDataflt, rgbaImg.cols, rgbaImg.rows,
						                        bbCUDA + (lastStart * 4), (n - lastStart) + 1, lastClass) )
						printf("detectnet-console:  failed to draw boxes\n");

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
			char c = cv::waitKey(5);
			if(c==27) // ESC is pressed
                break;
		}


		// update display
//        cv::Mat img;
//        img.LoadFromPixels(rgbaImg.cols, rgbaImg.rows, camData);
        char str[256];
        sprintf(str, "TensorRT %i.%i.%i | %s | %04.1f FPS", NV_TENSORRT_MAJOR, NV_TENSORRT_MINOR, NV_TENSORRT_PATCH, precisionTypeToStr(net->GetPrecision()), 0.0);
		cv::imshow(str, fltImg);
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
//        cudaFree(camDataflt);
	}

	printf("\ndetectnet-camera:  un-initializing video device\n");


	/*
	 * shutdown the camera device
	 */
//	if( camera != NULL )
//	{
//		delete camera;
//		camera = NULL;
//	}

//	if( display != NULL )
//	{
//		delete display;
//		display = NULL;
//	}
//	delete uchar* camData;
    cudaFree(camDataflt);

	printf("detectnet-camera:  video device has been un-initialized.\n");
	printf("detectnet-camera:  this concludes the test of the video device.\n");
	return 0;
}

