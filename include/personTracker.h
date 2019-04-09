#ifndef PERSONTRACKER_H
#define PERSONTRACKER_H

#include "cameraDetection.h"
#include "stopwatch.h"
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <thread>
#include <string>
#include <vector>


double distance3d(cv::Point3d pt1, cv::Point3d pt2) { return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.y-pt2.y,2) + pow(pt1.z-pt2.z,2)); }
double distance_xz(cv::Point3d pt1, cv::Point3d pt2) { return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.z-pt2.z,2)); }

struct trackedPerson
{
    personFrame last;
    double tolerance = 1.0;

    trackedPerson(personFrame _last=personFrame(), double _tolerance = 1.0)
      : last(_last),
        tolerance(_tolerance)
    {

    }
    void update(personFrame newPerson)
    {
        last = newPerson;
    }
    int update(const std::vector<personFrame>& personList)
    {
        double bestDist = 1e6;
        double dist     = 0;
        int    bestInd = -1;
        for(unsigned int i=0; i<personList.size(); i++)
        {
            dist = distance_xz(last.centroid, personList[i].centroid);
            if(dist < tolerance && dist < bestDist)
            {
                bestDist = dist;
                bestInd = i;
            }
        }
        if(bestInd > 0)
        {
            update(personList[bestInd]);
        }
        return bestInd;
    }
};






class personTracker
{
    public:
        personTracker(cv::Point3d defaultLocation, double _tolerance = 1.0);
        ~personTracker();

        /// User control functions
        void start();
        void stop();
        void show(){ camera.show(); }
        void hide(){ camera.hide(); }

        /// Utilities

        /// Getters and Setters
        bool isRunning() { return running; }
        void setTolerance(double tol);
        double getTolerance(){ return tolerance;}
        cv::Point3d getCentroid(){ return centroid; }

    protected:

    private:
        /// detection
        cameraDetection camera;
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

        /// people database
        std::vector<trackedPerson> people;
        trackedPerson default_person;
        double tolerance;
        cv::Point3d centroid;
};

#endif // PERSONTRACKER_H
