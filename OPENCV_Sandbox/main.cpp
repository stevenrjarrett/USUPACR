//#include "Matching.cpp"
//#include "Tracking_matching_Dual-camera.cpp"
//#include <opencv2/opencv.hpp>
//
//
//int main()
//{
//    cv::Mat img = cv::imread("birds.jpg");
//    cv::Mat img2;
////    img.copyTo(img2);
//    img2 = img(cv::Rect(500, 20, 200, 200));
//    cv::imwrite("birds2.jpg",img2);
//    return 0;
//}



#include <unistd.h>
#include <opencv2/opencv.hpp>
char* device = "/dev/video3";

int main()
{
        // Do some grabbing
    cv::VideoCapture video_capture;
    cv::Mat captured_image;
//    video_capture.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M','J','P','G'));
//    video_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
//    video_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
//    video_capture.set(cv::CAP_PROP_FPS, 30);

    std::cout << "Attempting to capture from device: " << device << std::endl;
    video_capture = cv::VideoCapture(device);

    if (!video_capture.isOpened())
    {
        std::cout << "Failed to open video source";
        return 1;
    }

    std::cout << "Device or file opened" << std::endl;
    video_capture >> captured_image;

//    usleep(10000000);
//
//    cv::VideoCapture webcam(1);
    cv::Mat img;

    while(true)
    {
//    if(!video_capture.read(img))
//        break;
        video_capture >> captured_image;

        cv::imshow("Webcam input", captured_image);

        int c = cv::waitKey(30);
        if(c == 27) // if ESC is pressed
            break;
    }
    return 0;
}//*/
