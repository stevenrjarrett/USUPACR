#ifndef CAMERADETECTION_H
#define CAMERADETECTION_H

#include <thread>

class cameraDetection
{
    public:
        cameraDetection(int _cameraNumber=3, double _framerate=30);
        ~cameraDetection();

        void start();
        void stop();
        void run();

        double Getframerate() { return framerate; }
        void Setframerate(double val) { framerate = val; }
        void Setdelay_time(unsigned int val) { delay_time = val; }
        std::vector<coordinate> Getlocations() { return locations; }

    protected:
        std::vector<coordinate> locations;

    private:
        double framerate;
        unsigned int delay_time;
        int cameraNumber;
        std::thread runningThread;
        bool isRunning;
};

#endif // CAMERADETECTION_H
