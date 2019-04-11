#include "personTracker.h"

double distance_xz(cv::Point3d pt1, cv::Point3d pt2) { return sqrt(pow(pt1.x-pt2.x,2) + pow(pt1.z-pt2.z,2)); }

personTracker::personTracker(cv::Point3d defaultLocation, double _tolerance)//, double _initial_tolerance = 1.0)
{
    show_color = true;
    show_depth = false;
    camera.showBoxes();
    lastTime = camera.getLastTime();
    init_wait_time = 3.0;
    default_person = trackedPerson(personFrame(defaultLocation, cv::Rect2d(camera.GetCOL_width()/2-30, camera.GetCOL_height()/2-45, 60, 90), 1.0), _tolerance);
//    tolerance = _tolerance;
    active = false;
    activity_timeout = .2; // number of seconds the controller is inactive before the class marks it as inactive

    // initialize threads. They will not run continuously unless running == true.
    this->stop_signal_recieved = false;
    running = false;
    runningThread = std::thread(&personTracker::run, this);
    activityThread = std::thread(&personTracker::activityChecker, this);
//    runningThread.join();
//    activityThread.join();
}

personTracker::~personTracker()
{
    //dtor
    stop();
    stop_signal_recieved = true;
    if(activityThread.joinable())
        activityThread.join();
    if(runningThread.joinable())
        runningThread.join();
}

void personTracker::activityChecker()
{
//    std::cout << "personTracker thread starting" << std::endl;
    active = false;
    while(!(this->stop_signal_recieved))
    {
//        std::cout << "stopwatch loop" << std::endl;
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
    while(!(this->stop_signal_recieved))
    {
//    std::cout << "personTracker Thread Started" << std::endl;
    // if I shouldn't be, don't run at all
    while(!running)
        usleep(1000);

    // initialize
    camera.start();

    people.clear();
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
//    people.clear();
    camera.stop();
    }
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
    std::cout << "Starting person tracker" << std::endl;

    running = true;
//    if(!runningThread.joinable())
//    {
//        runningThread = std::thread(&personTracker::run, this);
//        std::cout << "Person tracker threads started" << std::endl;
//    }
//    else
//        std::cout << "Attempted to start tracker, but it's already running.\n";
//    if(!activityThread.joinable())
//    {
//        activityThread = std::thread(&personTracker::activityChecker, this);
//        std::cout << "Person tracker activity checker threads started" << std::endl;
//    }
//    else
//        std::cout << "Attempted to start tracker activity watcher, but it's already running.\n";
}

void personTracker::stop()
{
    running = false;
//    std::cout << "Stopping tracker" << std::endl;
//    if(runningThread.joinable())
//        runningThread.join();
//    if(activityThread.joinable())
//        activityThread.join();
}
