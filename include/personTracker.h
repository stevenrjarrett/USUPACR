#ifndef PERSONTRACKER_H
#define PERSONTRACKER_H

#include "cameraDetection.h"
#include "stopwatch.h"
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <thread>
#include <string>
#include <vector>


//double distance3d(cv::Point3d pt1, cv::Point3d pt2) { return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.y-pt2.y,2) + pow(pt1.z-pt2.z,2)); }
double distance_xz(cv::Point3d pt1, cv::Point3d pt2);

struct trackedPerson
{
    personFrame last;
    double tolerance;

    trackedPerson(personFrame _last=personFrame(cv::Point3d(0, 0, 5)), double _tolerance = 1.0)
      : last(_last),
        tolerance(_tolerance)
    {

    }
    void update(personFrame newPerson)
    {
        last = newPerson;
//        std::cout << "New centroid: ("
//                  << std::setw(5) << last.centroid.x << " , "
//                  << std::setw(5) << last.centroid.y << " , "
//                  << std::setw(5) << last.centroid.z << ")" << std::endl;
    }
    int update(const std::vector<personFrame>& personList)
    {
        double bestDist = 1e6;
        double dist     = 0;
        int    bestInd = -1;
//        std::cout << "Person: " << std::setw(3) << personList.size() << " boxes, ";// << std::endl;
        for(unsigned int i=0; i<personList.size(); i++)
        {
            dist = distance_xz(last.centroid, personList[i].centroid);
            if(dist < bestDist)
            {
                bestDist = dist;
                bestInd = i;
            }
//            std::cout << "d=" << std::setw(5) << dist << ", ";
        }
//        std::cout << "BestDist = " << bestDist << ", bestInd = " << bestInd << ", ";
        if(bestDist < tolerance && bestInd >= 0)
        {
            update(personList[bestInd]);
        }
//        else
//            std::cout << std::endl;
        return bestInd;
    }
};






class personTracker
{
    public:
        personTracker(cv::Point3d defaultLocation, double _tolerance = 0.8);
        ~personTracker();

        /// User control functions
        void start();
        void stop();
        void show(){ camera.show(); }
        void hide(){ camera.hide(); }
        double init_wait_time; // in seconds

        /// Utilities

        /// Getters and Setters
        bool isRunning() { return running; }
        void setTolerance(double tol);
        double getTolerance(){ return tolerance;}
        cv::Point3d getCentroid(){ return centroid; }
        void setTimeout(double _timeout){ if(_timeout > 0) activity_timeout = _timeout;}
        bool found(){return active;}

        /// camera
        cameraDetection camera;
    protected:

    private:
        /// Activity Checker
        bool active;
        std::thread activityThread;
        stopwatch activityStopwatch;
        double activity_timeout; // number of seconds the controller is inactive before the class marks it as inactive
        void activityChecker();


        /// detection
        long long lastTime;

        /// show/hide camera feed
        std::string colorWindowName = "Color Video Feed";
        std::string depthWindowName = "Depth Video Feed";
        bool show_color;
        bool show_depth;

        /// running functions/variables
        bool running;
        void run();
        std::thread runningThread;
        bool stop_signal_recieved;

        /// people database
        std::vector<trackedPerson> people;
        trackedPerson default_person;
        double tolerance;
        cv::Point3d centroid;
};

#endif // PERSONTRACKER_H
