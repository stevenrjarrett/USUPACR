#include "personTracker.h"

double distance_xz(cv::Point3d pt1, cv::Point3d pt2) { return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.z-pt2.z,2)); }

personTracker::personTracker(cv::Point3d defaultLocation, double _tolerance)//, double _initial_tolerance = 1.0)
{
    show_color = true;
    show_depth = false;
    lastTime = camera.getLastTime();
    init_wait_time = 3.0;
    default_person = trackedPerson(personFrame(defaultLocation, cv::Rect2d(), 1.0), _tolerance);
//    tolerance = _tolerance;
    active = false;
    activity_timeout = .2; // number of seconds the controller is inactive before the class marks it as inactive

    running = false;
    runningThread = std::thread(&personTracker::run, this);
    activityThread = std::thread(&personTracker::activityChecker, this);
}

personTracker::~personTracker()
{
    //dtor
    stop();
}

void personTracker::activityChecker()
{
    active = false;
    while(running)
    {
        if(activityStopwatch.seconds() < activity_timeout)
            active = true;
        else
            active = false;
        usleep(10000);
    }
    active = false;
}

void personTracker::run()
{
    // if I shouldn't be, don't run at all
    if(!running)
        return;

    // initialize
    camera.start();
    people.push_back(default_person);

    // wait a few seconds for the person to get in position
    usleep(init_wait_time*1000000);

    while(running)
    {
        // wait for input
        while(camera.getLastTime() == lastTime)
            usleep(1000);
        lastTime = camera.getLastTime();

        // get last set of information
        std::vector<personFrame> pplData(camera.getPeople());

        // update people
//        for(unsigned int i=0; i<people.size(); i++)
//        {
//            people[i].update(pplData);
//        }
        int person_ind = people[0].update(pplData);
        if(person_ind >= 0)
        {
            activityStopwatch.reset();
        }

        centroid = people[0].last.centroid;

        if(show_color)
        {
            cv::rectangle(camera.colorMat, people[0].last.bb, COL_TEXT_COLOR, 5);
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

void personTracker::setTolerance(double tol)
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
