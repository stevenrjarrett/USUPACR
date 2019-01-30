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




#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture webcam(1);
    cv::Mat img;

    while(true)
    {
        if(!webcam.read(img))
            break;

        cv::imshow("Webcam input", img);

        int c = cv::waitKey(30);
        if(c == 27) // if ESC is pressed
            break;
    }
    return 0;
}//*/
