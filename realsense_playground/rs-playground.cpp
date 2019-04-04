// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

int main(int argc, char * argv[]) try
{
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;


    //Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;
    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 30);
//    cfg.enable_stream(RS2_STREAM_INFRARED, 1280, 720, RS2_FORMAT_Y8, 30);
//    cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);



    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start(cfg);

    using namespace cv;
//    const auto window_name = "Depth Image";
    const auto window_name2 = "Color Image";
//    namedWindow(window_name, WINDOW_AUTOSIZE);
    namedWindow(window_name2, WINDOW_AUTOSIZE);

    while (waitKey(1) < 0 && getWindowProperty(window_name2, WND_PROP_AUTOSIZE) >= 0)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera

        //Depth
//        rs2::frame depth = data.get_depth_frame();
//
//        // Query frame size (width and height)
//        const int w = depth.as<rs2::video_frame>().get_width();
//        const int h = depth.as<rs2::video_frame>().get_height();
//
//        // Create OpenCV matrix of size (w,h) from the colorized depth data
//
//        Mat image(Size(w, h), CV_16UC1, (void*)depth.get_data(), Mat::AUTO_STEP);

        //Color
        rs2::frame color = data.get_color_frame();

        // Query frame size (width and height)
        const int w2 = color.as<rs2::video_frame>().get_width();
        const int h2 = color.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data

        Mat color_image_raw(Size(640, 480), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
        Mat image2;
        cvtColor(color_image_raw, image2, COLOR_RGB2BGR);

        // Update the window with new data
//        imshow(window_name, image);
        imshow(window_name2, image2);
    }

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
