#ifndef USUPACR_MAIN
#define USUPACR_MAIN

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <fstream>
//#include <QApplication>

#include "personTracker.h"
#include "XBoxOne.h"
#include "mainwindow.h"

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Macros ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define EXECUTIVE_WAIT_TIME 200000 // microseconds
#define MOTORS_WAIT_TIME    200000 // microseconds
#define Max_Drive_Speed     255

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Global Variables ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

struct motorValues
{
    double left;
    double right;
    double brake;
    motorValues(int lm = 0, int rm = 0)
    {
        left = lm;
        right = rm;
    }
    motorValues& operator=(const motorValues& rhs)
    {
        left = rhs.left;
        right = rhs.right;
        brake = rhs.brake;
        return *this;
    }
    bool operator==(const motorValues& rhs)
    {
        return (left == rhs.left) &&
               (right == rhs.right) &&
               (brake == rhs.brake);
    }
    bool operator!=(const motorValues& rhs)
    {
        return (left != rhs.left) ||
               (right != rhs.right) ||
               (brake != rhs.brake);
    }
    motorValues& operator-=(const motorValues& rhs)
    {
        left -= rhs.left;
        right -= rhs.right;
        brake -= rhs.brake;
        return *this;
    }
    motorValues& operator+=(const motorValues& rhs)
    {
        left += rhs.left;
        right += rhs.right;
        brake += rhs.brake;
        return *this;
    }
};

motorValues motors_target;
motorValues motors_actual;
//double lSpeed = 0;
//double rSpeed = 0;
bool EStop = false;
bool Manual_Only = false;
bool autonomous_mode = false;
double follow_distance = 5.0;
double distance_tolerance = 2.0;
double max_speed = 255; // 255 is the max value you can send. Set this lower for slower
double max_acceleration = 0.8; // 1 is for 0 to max_speed in 1 second;
double autonomous_max_speed = 200; // 255 is the max value you can send. Set this lower for slower
double motor_speed_limiter = 1.0; // a value from 0-1, setting the maximum speed.
double autonomous_x_tolerance = 0.5;
bool enable_soft_start = true;
double drive_increment = max_speed * max_acceleration * ((double)MOTORS_WAIT_TIME / 1000000.0);
double controllerTimeout = 1.5; // seconds


/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Header ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

bool stop_signal_recieved = false;

//# include "XBoxOne.hpp"

template<class T>
T abs_val(T val)
{
    if(val < 0)
        val *= -1;
    return val;
}


void sig_handler(int signo);


void updateMotorValues();

void sendMotorValues();
void motorUpdator();





/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////  Main  ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void mainProgram(QApplication *GUI_app, MainWindow *GUI);

int main(int argc, char** argv);

motorValues ConvertToArcade ( int x1 , int y1 ); // Converts joystick input to holonomic drive output in the array MTarget[]


#endif // USUPACR_MAIN
