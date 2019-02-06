#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#define JOY_DEV "/dev/input/js0"

using namespace std;

int main()
{
  int joy_fd(-1), num_of_axis(0), num_of_buttons(0);
  char name_of_joystick[80];
  vector<char> joy_button;
  vector<int> joy_axis;

  if((joy_fd=open(JOY_DEV,O_RDONLY)) < 0)
  {
    cerr<<"Failed to open "<<JOY_DEV<<endl;
    return -1;
  }

  ioctl(joy_fd, JSIOCGAXES, &num_of_axis);
  ioctl(joy_fd, JSIOCGBUTTONS, &num_of_buttons);
  ioctl(joy_fd, JSIOCGNAME(80), &name_of_joystick);

  joy_button.resize(num_of_buttons,0);
  joy_axis.resize(num_of_axis,0);

  cout<<"Joystick: "<<name_of_joystick<<endl
    <<"  axis: "<<num_of_axis<<endl
    <<"  buttons: "<<num_of_buttons<<endl;

  fcntl(joy_fd, F_SETFL, O_NONBLOCK);   // using non-blocking mode

  while(true)
  {
    js_event js;

    read(joy_fd, &js, sizeof(js_event));

    switch (js.type & ~JS_EVENT_INIT)
    {
    case JS_EVENT_AXIS:
      if((int)js.number>=joy_axis.size())  {cerr<<"err:"<<(int)js.number<<endl; continue;}
      joy_axis[(int)js.number]= js.value;
      break;
    case JS_EVENT_BUTTON:
      if((int)js.number>=joy_button.size())  {cerr<<"err:"<<(int)js.number<<endl; continue;}
      joy_button[(int)js.number]= js.value;
      break;
    }

    cout<<"axis/1000: ";
    for(size_t i(0);i<joy_axis.size();++i)
      cout<<" "<<setw(3)<<joy_axis[i]/1000;
    cout<<endl;

    cout<<"  button: ";
    for(size_t i(0);i<joy_button.size();++i)
      cout<<" "<<(int)joy_button[i];
    cout<<endl;

    usleep(100);
  }

  close(joy_fd);
  return 0;
}






//#include <unistd.h>
//#include <iostream>
//
//#include <sys/stat.h>
//#include <limits.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/fcntl.h>
//#include <unistd.h>
//#include <errno.h>
//
////    where js_event is defined as
//
////    	struct js_event {
////    		__u32 time;     /* event timestamp in milliseconds */
////    		__s16 value;    /* value */
////    		__u8 type;      /* event type */
////    		__u8 number;    /* axis/button number */
////    	};
//
//int main(int argc, char** argv)
//{
//    while(true)
//    {
//        int fd = open("/dev/input/event7", O_RDONLY);
//
//        js_event e;
//        read (fd, &e, sizeof(e));
//
//        std::cout << "Time:\t" << e.time << '\n'
//                  << "Value:\t" << e.value << '\n'
//                  << "Type:\t"  << e.type << '\n'
//                  << "Number:\t"<< e.number << std::endl;
//        usleep(10000);
//    }
//
//    return 0;
//}





//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <unistd.h>
//#include "libenjoy/src/libenjoy.h"
//#include "libenjoy/src/libenjoy_linux.h"
//#include "libenjoy/src/libenjoy.c"
//#include "libenjoy/src/libenjoy_linux.c"
//
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Macros ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
//#define EXECUTIVE_WAIT_TIME 30000 // microseconds
//
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Global Variables ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
//double lSpeed = 0;
//double rSpeed = 0;
//bool EStop = false;
//bool Manual_Only = false;
//
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Header ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//
////# include "XBoxOne.hpp"
//
//int main()
//{
//    std::cout << "USU PACR Control Program" << std::endl;
//
//    /// Initialize
//        // Joystick
////        XBoxOne controller("/dev/input/event8");
////        int controller = open("/dev/input/event8", 0_RDONLY);
////        if(controller == 0)
////        {
////            }
//        // IMU
//        // Cameras/autonomous function
//        // Communication with Arduino
//
//
//    // For now, we're only doing manual control.
//    Manual_Only = true;
//
//    /// Executive decision making function
//    while(true)
//    {
//        if(!EStop)
//        {
//            //read inputs
//            //decide what to do and set variables
//        }
//        else
//        {
//            lSpeed = 0;
//            rSpeed = 0;
//        }
//        //wait for a little bit
//        usleep(EXECUTIVE_WAIT_TIME);
//    }
//    return 0;
//}
//
//
//
