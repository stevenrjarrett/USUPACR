#include <iostream>
#include <unistd.h>
#include <fstream>

#include "XBoxOne.h"
#include "pacrControl.h"

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Macros ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define EXECUTIVE_WAIT_TIME 200000 // microseconds
#define MOTORS_WAIT_TIME    200000 // microseconds
#define Max_Drive_Speed     255

bool EStop = false;
bool Manual_Only = false;
double distance_tolerance = 2.0;
double max_speed = 255; // 255 is the max value you can send. Set this lower for slower
double max_acceleration = 0.8; // 1 is for 0 to max_speed in 1 second;
double autonomous_max_speed = 200; // 255 is the max value you can send. Set this lower for slower
double motor_speed_limiter = 1.0; // a value from 0-1, setting the maximum speed.
double autonomous_x_tolerance = 0.5;
bool enable_soft_start = true;
double drive_increment = max_speed * max_acceleration * ((double)MOTORS_WAIT_TIME / 1000000.0);
double controllerTimeout = 1.5; // seconds

int main()
{
    bool wasActive = false;

    std::cout << "USU PACR Control Program" << std::endl;
    usleep(10000); // wait to make sure the main window is set up.

    /// Initialize
        // Joystick
        XBoxOne controller;
        controller.setTimeout(controllerTimeout);
        // Cameras/autonomous function
        personTracker tracker(cv::Point3d(0, 0, follow_distance));
        // Communication with Arduino
        pacrControl motors;
        motors.set_autonomous_max_speed(autonomous_max_speed);
        motors.set_max_acceleration(max_acceleration);
        motors.set_max_speed(max_speed);
        // enable signal catcher
        if( signal(SIGINT, sig_handler) == SIG_ERR )
            printf("\ncan't catch SIGINT\n");

    // For now, we're only doing manual control.
    Manual_Only = true;






    /// Executive decision making function
    bool wasRunning = false;

    while(!stop_signal_recieved)
    {
        // if the e-stop was not pressed, do normal stuff
        if(!EStop)
        {
            // reset 1st stop signal (for reporting purposes
            if(!wasRunning)
            {
                std::cout << "Running" << std::endl;
                wasRunning = true;
            }


            //Manual Control


            else if(controller.isConnected() && controller.isActive())
            {
                wasActive = true;
                //read inputs
                //decide what to do and set variables
                motors.target = ConvertToArcade(controller.L_x()*max_speed, -controller.L_y()*max_speed);
                motors.target.brake = (controller.lTrigger() + 1.0) / 2.0;
                motors.target.autonomous = controller.Y();
            }
            else
            {
                if(wasActive)
                {
                    wasActive = false;
                    std::cout << "Controller inactive or disconnected" << std::endl;
                }
                motors.target.left = 0;
                motors.target.right = 0;
                motors.target.autonomous = false;
            }
        }
        else
        {
            motors.target.left = 0;
            motors.target.right = 0;
            if(wasRunning)
            {
                wasRunning = false;
                std::cout << "E-stop engaged" << std::endl;
//                if(!controller.isConnected())
//                    std::cout << "Controller disconnected, stopping motors.target" << std::endl;
//                if(!controller.isActive())
//                    std::cout << "Controller inactive, stopping motors.target" << std::endl;
            }
        }

//        sendMotorValues();
//            controller.printALL();
//        std::cout << "Values sent: " << motors.target.left  << '\t' << motors.target.right << std::endl;

        //wait for a little bit
        usleep(EXECUTIVE_WAIT_TIME);
    }
    std::cout << "Ending program. Goodbye!" << std::endl;
    stop_signal_recieved = true;
//    bool hasGuiApp = (qobject_cast<QApplication*>(QCoreApplication::instance())!=0);
//    if(GUI_app.instance()!=0)
//        GUI_app->quit();

//    return 0;


//    std::exit(1);
}
