#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <fstream>

#include "personTracker.h"
#include "XBoxOne.h"

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
double max_acceleration = 1.0; // 1 is for 0 to max_speed in 1 second;
double autonomous_max_speed = 127; // 255 is the max value you can send. Set this lower for slower
double motor_speed_limiter = 0.5; // a value from 0-1, setting the maximum speed.
double autonomous_x_tolerance = 0.5;
bool enable_soft_start = true;
double drive_increment = max_speed * max_acceleration * ((double)MOTORS_WAIT_TIME / 1000000.0);

std::fstream motorArduino;


/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Header ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


motorValues ConvertToArcade ( int x1 , int y1 );

void sendSerial(int val);
bool stop_signal_recieved = false;

//# include "XBoxOne.hpp"

template<class T>
T abs_val(T val)
{
    if(val < 0)
        val *= -1;
    return val;
}


void sig_handler(int signo)
{
	if( signo == SIGINT )
	{
		std::cout << "received SIGINT" << std::endl;
		stop_signal_recieved = true;
	}
}


void updateMotorValues()
{
    // Set brake
    motors_actual.brake = motors_target.brake;

    //use soft-start to set motors
    motorValues mtrDiff = motors_target,
                aMtrDiff;
    mtrDiff -= motors_actual;
    aMtrDiff.left  = abs_val(MtrDiff.left );
    aMtrDiff.right = abs_val(MtrDiff.right);
    aMtrDiff.brake = abs_val(MtrDiff.brake);

//Drive
    if ( (aMtrDiff.left > drive_increment || aMtrDiff.right > drive_increment ) && enable_soft_start)
    {
        double max_aDiff = std::max(aMtrDiff.left, aMtrDiff.right);

        motors_actual.left += ( drive_increment * mtrDiff.left) / max_aDiff;
        motors_actual.right += ( drive_increment * mtrDiff.right) / max_aDiff;
    }
    else
    {
        motors_actual.left = motors_target.left;
        motors_actual.right = motors_target.right;
    }
    std::cout << "ltarget = " << motors_target.left << ", lActual = " << motors_actual.left << ", mtrDiff = " << mtrDiff.left << ", aMtrDiff = " << aMtrDiff.left << std::endl;
}

void sendMotorValues()
{
    motorArduino << (int)(motors_actual.left  * motor_speed_limiter) << std::endl;
    motorArduino << (int)(motors_actual.right * motor_speed_limiter) << std::endl;
//    motors_actual = motors_target;
//    std::cout << "sent values: l=" << (int)(motors_target.left  * motor_speed_limiter)
//                         << ", r=" << (int)(motors_target.right * motor_speed_limiter) << std::endl;
}

void motorUpdator()
{
    motorArduino.open("/dev/ttyACM0", std::ios_base::out );
    while(!stop_signal_recieved)
    {
        updateMotorValues();
        sendMotorValues();
        usleep(MOTORS_WAIT_TIME);
    }
}





/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////  Main  ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    bool wasActive = false;

    std::cout << "USU PACR Control Program" << std::endl;

    /// Initialize
        // Joystick
        XBoxOne controller;
        controller.setTimeout(1.5);
//        controller.start();
//        int controller = open("/dev/input/event8", 0_RDONLY);
//        if(controller == 0)
//        {
//            }
        // IMU
        // Cameras/autonomous function
//        cameraDetection cam;
        personTracker tracker(cv::Point3d(0, 0, follow_distance));
        // Communication with Arduino
        std::thread motorThread = std::thread(motorUpdator);

        // enable signal catcher
        if( signal(SIGINT, sig_handler) == SIG_ERR )
            printf("\ncan't catch SIGINT\n");

    // For now, we're only doing manual control.
    Manual_Only = true;






    /// Executive decision making function
    bool wasRunning = false;

    while(!stop_signal_recieved)
    {
//        std::cout << "Beginning of executive loop" << std::endl;
        // get input from controller for the E-stops
        if(controller.wp_rBumper())
        {
            EStop = false;
        }
        if(controller.wp_rTrigger())
        {
            EStop = true;
            autonomous_mode = false;
        }
        if(controller.wp_lBumper())
        {
            motors_target.left = 0;
            motors_target.right = 0;
//            sendMotorValues();
            break;
        }
        // if the e-stop was not pressed, do normal stuff
        if(!EStop)
        {
            // reset 1st stop signal (for reporting purposes
            if(!wasRunning)
            {
                std::cout << "Running" << std::endl;
                wasRunning = true;
            }
            // Act based on autonomous / user control
            if(autonomous_mode)
            {
                if(controller.wp_B() || abs_val(controller.L_x()) > 0.1 || abs_val(controller.L_y()) > 0.1 || !controller.isActive() || !controller.isConnected())
                {
                    autonomous_mode = false;
                    motors_target.left = 0;
                    motors_target.right = 0;
//                    sendMotorValues();
                    std::cout << "disabling autonomous mode. User control only." << std::endl;
                    tracker.stop();
                    controller.wp_A();
                }
                else if(tracker.found())
                {
                    /// TODO
                    double turningVal = (double)tracker.getCentroid().x * autonomous_max_speed / autonomous_x_tolerance; // positive to turn right, negative to turn left.
                    if(abs_val(turningVal) > autonomous_max_speed)
                        turningVal = turningVal / abs_val(turningVal) * autonomous_max_speed;
                    double speedVal   = 0; // positive for forward, negative for backward
                    if(tracker.getCentroid().z >= (follow_distance - distance_tolerance))
                    {
                        speedVal = (tracker.getCentroid().z - follow_distance + distance_tolerance) / (2*distance_tolerance);
                        // speedVal should be in a value between 0 and 1 if the person is at the target following
                        // distance +- the distance tolerance.
                        speedVal *= max_speed;

                        // If the person's farther away, fix the speed value.
                        if(speedVal > max_speed)
                            speedVal = max_speed;
                    }
                    motors_target = ConvertToArcade(turningVal, speedVal);
                }
                else
                {
                    motors_target.left = 0;
                    motors_target.right = 0;
                }
            }
            else
            {
                if(controller.wp_A())
                {
                    autonomous_mode = true;
                    motors_target.left = 0;
                    motors_target.right = 0;
//                    sendMotorValues();
                    controller.wp_B();
                    std::cout << "Beginning autonomous mode" << std::endl;
                    tracker.start();
                }
                else if(controller.isConnected() && controller.isActive())
                {
                    wasActive = true;
                    //read inputs
                    //decide what to do and set variables
                    motors_target = ConvertToArcade(controller.L_x()*max_speed, -controller.L_y()*max_speed);
                }
                else
                {
                    if(wasActive)
                    {
                        wasActive = false;
                        std::cout << "Controller inactive or disconnected" << std::endl;
                    }
                    motors_target.left = 0;
                    motors_target.right = 0;
                }
            }
        }
        else
        {
            motors_target.left = 0;
            motors_target.right = 0;
            if(wasRunning)
            {
                wasRunning = false;
                std::cout << "E-stop engaged" << std::endl;
//                if(!controller.isConnected())
//                    std::cout << "Controller disconnected, stopping motors_target" << std::endl;
//                if(!controller.isActive())
//                    std::cout << "Controller inactive, stopping motors_target" << std::endl;
            }
        }

//        sendMotorValues();
//            controller.printALL();
//        std::cout << "Values sent: " << motors_target.left  << '\t' << motors_target.right << std::endl;

        //wait for a little bit
        usleep(EXECUTIVE_WAIT_TIME);
    }
    std::cout << "Ending program. Goodbye!" << std::endl;
    stop_signal_recieved = true;
    if(motorThread.joinable())
        motorThread.join();
//    cam.stop();
//    controller.stop();
    return 0;
}










/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Functions //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


motorValues ConvertToArcade ( int x1 , int y1 ) // Converts joystick input to holonomic drive output in the array MTarget[]
{
  motorValues mtrs;
  int A;
  int B;
  int Ax1;
  int Ay1;
  Ax1 = abs_val(x1);//Getting absolute values so I can use them in if-statements
  Ay1 = abs_val(y1);
  A = Ax1 + Ay1;//The Common Divisor
  if (A > 3) // This if-statemend does two things: First, if you'll notice, A is a divisor later on. if A = 0, then the program would stop working. Second, setting the minimum value to 3 eliminates motor hum.
  {
    B = (Ax1 > Ay1)?(Ax1):(Ay1);//Finds the greatest value of the variables Ax1 and Ay1, and stores it in B. Done using ternary if-else statements.
		B = (B > Max_Drive_Speed)?(Max_Drive_Speed):(B);//If B is greater than the maximum drive speed, then set it to the max value.
    mtrs.left = (( y1 + x1) * B) / A; //These four lines of code are the meat of this function. everything else is just to make them work.
    mtrs.right = (( y1 - x1) * B) / A;// Multiplying by B / A ensures that no motor value exceeds the greatest input value (which is 127 or -127 for joystick input).
  }
  else
  {
    mtrs.left = 0;
    mtrs.right = 0;
  }
  return mtrs;
}
