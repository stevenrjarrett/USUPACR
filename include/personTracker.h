#ifndef PERSONTRACKER_H
#define PERSONTRACKER_H

#include "cameraDetection.h"


struct trackedPerson
{
    personFrame last;
    double tolerance = 1.0;

    void update(personFrame newPerson)
    {
        last = newPerson;
    }
    void update(const std::vector<personFrame>& personList)
    {
        double bestDist = 1e6;
        double dist     = 0;
        int    bestInd = -1;
        for(unsigned int i=0; i<personList.size(); i++)
        {
            dist = distance3d(last, personList[i]);
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
    }
};

class personTracker
{
    public:
        personTracker();
        ~personTracker();

        /// User control functions
        void start();
        void stop();
        void show(){ show_color = true; }
        void hide(){ show_color = false; if(isRunning) cv::destroyWindow(colorWindowName);}

        /// Utilities
        double distance3d(cv::Point3d pt1, cv::Point3d pt2)
        {
            return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.y-pt2.y,2) + pow(pt1.z-pt2.z,2));
        }

        /// Getters and Setters
        bool isRunning() { return running; }

    protected:

    private:
        bool running;
        cameraDetection camera;
};

#endif // PERSONTRACKER_H
