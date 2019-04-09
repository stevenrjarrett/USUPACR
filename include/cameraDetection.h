#ifndef CAMERADETECTION_H
#define CAMERADETECTION_H

/// Includes
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <string>


//#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <vector>
#include <thread>

#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/cudaFont.h>

#include <jetson-inference/detectNet.h>

/// Macros
#define CVT_COLOR_TO_DEPTH 0
#define CVT_DEPTH_TO_COLOR 1

#define COL_TEXT_COLOR cv::Scalar(0, 0, 0)
#define DEPTH_TEXT_COLOR cv::Scalar( 65535/2, 65535/2, 65535/2 )



class cameraDetection
{
    public:
        cameraDetection();
        ~cameraDetection();


        /// User control functions
        void resetCamera();
        void start();
        void stop();
        void showDepth(){ show_depth = true; }
        void hideDepth(){ show_depth = false; if(isRunning) cv::destroyWindow(depthWindowName);}
        void show(){ show_color = true; }
        void hide(){ show_color = false; if(isRunning) cv::destroyWindow(colorWindowName);}

        /// Utilities
        void bboxFix( const cv::Mat& img, cv::Rect2d& box);
        cv::Point3d getCentroid(cv::Mat &depthMat, const rs2::depth_frame& dframe, cv::Rect drect);
        cv::Rect2d cvt_bb(cv::Rect2d bb, int cvt_type);

        /// Getters and Setters
        int Getmax_fps() { return max_fps; }
        void Setmax_fps(int val) { max_fps = val; resetCamera(); }
        double GetDEPTH_Hor_Field_of_View() { return DEPTH_Hor_Field_of_View; }
//        void SetDEPTH_Hor_Field_of_View(double val) { DEPTH_Hor_Field_of_View = val; }
        double GetDEPTH_Ver_Field_of_View() { return DEPTH_Ver_Field_of_View; }
//        void SetDEPTH_Ver_Field_of_View(double val) { DEPTH_Ver_Field_of_View = val; }
        double GetDEPTH_width() { return DEPTH_width; }
        void SetDEPTH_width(double val) { DEPTH_width = val;  resetCamera(); }
        double GetDEPTH_height() { return DEPTH_height; }
        void SetDEPTH_height(double val) { DEPTH_height = val;  resetCamera(); }
        int GetDEPTH_numPixels() { return DEPTH_numPixels; }
//        void SetDEPTH_numPixels(int val) { DEPTH_numPixels = val; }
        double Getdepth_scale() { return depth_scale; }
        void Setdepth_scale(double val) { depth_scale = val; }
        double GetCOL_Hor_Field_of_View() { return COL_Hor_Field_of_View; }
//        void SetCOL_Hor_Field_of_View(double val) { COL_Hor_Field_of_View = val; }
        double GetCOL_Ver_Field_of_View() { return COL_Ver_Field_of_View; }
//        void SetCOL_Ver_Field_of_View(double val) { COL_Ver_Field_of_View = val; }
        double GetCOL_width() { return COL_width; }
        void SetCOL_width(double val) { COL_width = val; resetCamera(); }
        double GetCOL_height() { return COL_height; }
        void SetCOL_height(double val) { COL_height = val; resetCamera(); }
        int GetCOL_numPixels() { return COL_numPixels; }
//        void SetCOL_numPixels(int val) { COL_numPixels = val; }
        double Getx_color_to_depth_conversion_factor() { return x_color_to_depth_conversion_factor; }
//        void Setx_color_to_depth_conversion_factor(double val) { x_color_to_depth_conversion_factor = val; }
        double Gety_color_to_depth_conversion_factor() { return y_color_to_depth_conversion_factor; }
//        void Sety_color_to_depth_conversion_factor(double val) { y_color_to_depth_conversion_factor = val; }


    protected:

    private:
        /// Frame information
        int max_fps;

        std::string depthWindowName;
        double DEPTH_Ver_Field_of_View;
        double DEPTH_width;
        double DEPTH_height;
        double DEPTH_Hor_Field_of_View;
        int DEPTH_numPixels;
        double depth_scale;

        std::string colorWindowName;
        double COL_Hor_Field_of_View;
        double COL_Ver_Field_of_View;
        double COL_width;
        double COL_height;
        int COL_numPixels;

        /// Conversion factors
        double x_color_to_depth_conversion_factor;
        double y_color_to_depth_conversion_factor;

        /// runtime variables
        bool isRunning;
        bool show_color;
        bool show_depth;

        ///
        std::thread runningThread;
        void run();
};

#endif // CAMERADETECTION_H
