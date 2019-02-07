#include <iostream>
//#include <opencv2/opencv.hpp>
#include <unistd.h>
#include "include/XBoxOne.h"
#include "src/XBoxOne.cpp"

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Macros ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define EXECUTIVE_WAIT_TIME 100000 // microseconds

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Global Variables ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

double lSpeed = 0;
double rSpeed = 0;
bool EStop = false;
bool Manual_Only = false;

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Header ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//# include "XBoxOne.hpp"

int main()
{
    std::cout << "USU PACR Control Program" << std::endl;

    /// Initialize
        // Joystick
        XBoxOne controller;
//        controller.start();
//        int controller = open("/dev/input/event8", 0_RDONLY);
//        if(controller == 0)
//        {
//            }
        // IMU
        // Cameras/autonomous function
        // Communication with Arduino


    // For now, we're only doing manual control.
    Manual_Only = true;

    /// Executive decision making function
    while(true)
    {
        //wait for a little bit
        usleep(EXECUTIVE_WAIT_TIME);
//        std::cout << "Beginning of executive loop" << std::endl;
        if(!EStop)
        {
            //read inputs
            controller.printALL();
            //decide what to do and set variables
            if(controller.lBumper())
                break;
        }
        else
        {
            lSpeed = 0;
            rSpeed = 0;
        }
    }
    controller.stop();
    return 0;
}



