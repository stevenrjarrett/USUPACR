#include <iostream>
//#include <opencv2/opencv.hpp>
#include <unistd.h>
#include "include/XBoxOne.h"
#include "src/XBoxOne.cpp"
#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Macros ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#define EXECUTIVE_WAIT_TIME 200000 // microseconds
#define Max_Drive_Speed 255

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Global Variables ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

struct motorValues
{
    int left;
    int right;
    motorValues(int lm = 0, int rm = 0)
    {
        left = lm;
        right = rm;
    }
};

motorValues motors;
double lSpeed = 0;
double rSpeed = 0;
bool EStop = false;
bool Manual_Only = false;


/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Header ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


motorValues ConvertToArcade ( int x1 , int y1 );

void sendSerial(int val);

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
        std::ofstream motorArduino("/dev/ttyACM0");


    // For now, we're only doing manual control.
    Manual_Only = true;





    /// Executive decision making function
    while(true)
    {
//        std::cout << "Beginning of executive loop" << std::endl;

        if(!EStop && controller.isConnected())
        {
            //read inputs
            motors = ConvertToArcade(controller.L_x()*255, -controller.L_y()*255);
            //decide what to do and set variables
            if(controller.lBumper())
                break;
        }
        else
        {
            motors.left = 0;
            motors.right = 0;
        }

        motorArduino << motors.left << std::endl;
        motorArduino << motors.right << std::endl;
//            controller.printALL();
        std::cout << "Values sent: " << motors.left  << '\t' << motors.right << std::endl;

        //wait for a little bit
        usleep(EXECUTIVE_WAIT_TIME);
    }
    controller.stop();
    return 0;
}


motorValues ConvertToArcade ( int x1 , int y1 ) // Converts joystick input to holonomic drive output in the array MTarget[]
{
  motorValues mtrs;
  int A;
  int B;
  int Ax1;
  int Ay1;
  Ax1 = abs(x1);//Getting absolute values so I can use them in if-statements
  Ay1 = abs(y1);
  A = Ax1 + Ay1;//The Common Divisor
  if (A > 10) // This if-statemend does two things: First, if you'll notice, A is a divisor later on. if A = 0, then the program would stop working. Second, setting the minimum value to 3 eliminates motor hum.
  {
    B = (Ax1 > Ay1)?(Ax1):(Ay1);//Finds the greatest value of the variables Ax1 and Ay1, and stores it in B. Done using ternary if-else statements.
		B = (B > Max_Drive_Speed)?(Max_Drive_Speed):(B);//If B is greater than the maximum drive speed, then set it to the max value.
    mtrs.left = (( y1 + x1) * B) / A;//These four lines of code are the meat of this function. everything else is just to make them work.
    mtrs.right = (( y1 - x1) * B) / A;// Multiplying by B / A ensures that no motor value exceeds the greatest input value (which is 127 or -127 for joystick input).
  }
  else
  {
    mtrs.left = 0;
    mtrs.right = 0;
  }
  return mtrs;
}
