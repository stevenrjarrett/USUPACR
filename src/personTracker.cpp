#include "personTracker.h"

personTracker(cv::Point3d defaultLocation, double _tolerance = 1.0)//, double _initial_tolerance = 1.0)
{
    show_color = false;
    show_depth = false;
    lastTime = camera.getLastTime();
    default_person = personFrame(defaultLocation, cv::Rect2d(), 1.0);
    default_person.tolerance = _tolerance;
//    tolerance = _tolerance;

    running = false;
    runningThread = std::thread(&personTracker::run, this);
}

personTracker::~personTracker()
{
    //dtor
    stop();
}



void personTracker::run()
{
    // if I shouldn't be, don't run at all
    if(!running)
        return;

    // initialize
    camera.start();
    people.push_back(default_person);
    while(running)
    {
        // wait for input
        while(camera.lastUpdated() == lastTime)
            usleep(1000);
        lastTime = camera.getLastTime();

        // get last set of information
        std::vector<personFrame> pplData(camera.getPeople());

        // update people
        for(unsigned int i=0; i<people.size(); i++)
        {
            people[i].update(pplData);
        }

        centroid = people[0].centroid;

        if(show_color)
        {
            cv::imshow(colorWindowName, camera.colorMat);
        }
        if(show_depth)
        {
            cv::imshow(depthWindowName, camera.depthMat);
        }

    }
    people.clear();
    camera.stop();
}

void setTolerance(double tol)
{
    tolerance = tol;
    for(unsigned int i=0; i<people.size(); i++)
    {
        people[i].tolerance = tolerance;
    }
}

void personTracker::start()
{
    if(!running && runningThread.joinable())
    {
        running = true;
        runningThread = std::thread(&personTracker::run, this);
    }
    else
        std::cout << "Attempted to start tracker, but it's already running.\n";
}

void personTracker::stop()
{
    running = false;
    std::cout << "Stopping tracker" << std::endl;
    try
    {
        runningThread.join();
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: Tracker stopped twice (error caught)" << std::endl;
    }
}
