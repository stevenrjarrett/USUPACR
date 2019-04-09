#include "XBoxOne.h"
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <pthread.h>
//#include "../libenjoy/src/libenjoy.h"
//#include "../libenjoy/src/libenjoy_linux.h"
//
//#include "../libenjoy/src/libenjoy.c"
//#include "../libenjoy/src/libenjoy_linux.c"

// This tels msvc to link agains winmm.lib. Pretty nasty though.
//#pragma comment(lib, "winmm.lib")

XBoxOne::XBoxOne()
{
    isRunning = true;
    ctx = libenjoy_init(); // initialize the library
    connected = false;
    pollingThread = std::thread(&XBoxOne::run, this);
}

XBoxOne::~XBoxOne()
{
    // Frees memory allocated by that joystick list. Do not forget it!
    libenjoy_free_info_list(info);//#include
}

void XBoxOne::start()
{
    if(!isRunning && pollingThread.joinable())
    {
        isRunning = true;
        pollingThread = std::thread(&XBoxOne::run, this);
    }
    else
        std::cout << "Attempted to start xbox controller, but it's already running.\n";
}

void XBoxOne::stop()
{
    isRunning = false;
    std::cout << "Stopping controller" << std::endl;
    pollingThread.join();
    std::cout << "Controller stopped" << std::endl;
}

bool XBoxOne::isConnected()
{
    return connected;
}

void XBoxOne::run()
{
    // Updates internal list of joysticks. If you want auto-reconnect
    // after re-plugging the joystick, you should call this every 1s or so
    libenjoy_enumerate(ctx);

    // get list with available joysticks. structs are
    // typedef struct libenjoy_joy_info_list {
    //     uint32_t count;
    //     libenjoy_joy_info **list;
    // } libenjoy_joy_info_list;
    //
    // typedef struct libenjoy_joy_info {
    //     char *name;
    //     uint32_t id;
    //     char opened;
    // } libenjoy_joy_info;
    //
    // id is not linear (eg. you should not use vector or array),
    // and if you disconnect joystick and then plug it in again,
    // it should have the same ID
    info = libenjoy_get_info_list(ctx);

    //Wait until you have a connection
    while(info->count == 0 && isRunning)
    {
        std::cout << "Waiting for connection:" << std::endl;
        usleep(1000000);
        libenjoy_enumerate(ctx);
        info = libenjoy_get_info_list(ctx);
    }
    connected = true;

    // set up and read stuff.
    libenjoy_joystick *joy;
    printf("Opening joystick %s...", info->list[0]->name);
    joy = libenjoy_open_joystick(ctx, info->list[0]->id);
    if(joy)
    {
        int counter = 0;
        libenjoy_event ev;

        printf("Opened Joystick!\n");
        printf("Axes: %d btns: %d\n", libenjoy_get_axes_num(joy),libenjoy_get_buttons_num(joy));
//        std::cout << "Am I running: " << isRunning << std::endl;
        while(isRunning)
        {
            // Value data are not stored in library. if you want to use
            // them, you have to store them

            // That's right, only polling available
            while(libenjoy_poll(ctx, &ev))
            {
                switch(ev.type)
                {
                case LIBENJOY_EV_AXIS:
//                    printf("%u: axis %d val %d\n", ev.joy_id, ev.part_id, ev.data);
                    setAxis(ev.part_id, ev.data);
                    break;
                case LIBENJOY_EV_BUTTON:
//                    printf("%u: button %d val %d\n", ev.joy_id, ev.part_id, ev.data);
                    setBtn(ev.part_id, ev.data);
                    break;
                case LIBENJOY_EV_CONNECTED:
//                    printf("%u: status changed: %d\n", ev.joy_id, ev.data);
                    connected = bool(ev.data);
                    break;
                }
            }
            usleep(50000);
            counter += 50;
            if(counter >= 1000)
            {
                libenjoy_enumerate(ctx);
                counter = 0;
            }
        }

        // Joystick is really closed in libenjoy_poll or libenjoy_close,
        // because closing it while libenjoy_poll is in process in another thread
        // could cause crash. Be sure to call libenjoy_poll(ctx, NULL); (yes,
        // you can use NULL as event) if you will not poll nor libenjoy_close
        // anytime soon.
        libenjoy_close_joystick(joy);
    }
    else
        printf("Failed!\n");
}

void XBoxOne::setAxis(int id, int val)
{
    switch(id)
    {
    case 0:
        joyL_x = val;
        break;
    case 1:
        joyL_y = val;
        break;
    case 2:
        joyLTrigger = val;
        break;
    case 3:
        joyR_x = val;
        break;
    case 4:
        joyR_y = val;
    case 5:
        joyRTrigger = val;
    case 6:
        if(val<0) // left arrow
        {
            btnLEFT  = true;
            btnRIGHT = false;
        }
        else if(val>0) // right arrow
        {
            btnLEFT  = false;
            btnRIGHT = true;
        }
        else // val==0 // no arrow pressed
        {
            btnLEFT  = false;
            btnRIGHT = false;
        }
        break;
    case 7:
        if(val<0) // up arrow
        {
            btnDOWN = false;
            btnUP   = true;
        }
        else if(val>0) // down arrow
        {
            btnDOWN = true;
            btnUP   = false;
        }
        else // val==0 // no arrow pressed
        {
            btnDOWN = false;
            btnUP   = false;
        }
        break;
    }

}

void XBoxOne::setBtn(int id, int val)
{
    switch(id)
    {
    case 0: // A
        btnA = val;
        break;
    case 1: // B
        btnB = val;
        break;
    case 2: // X
        btnX = val;
        break;
    case 3: // Y
        btnY = val;
        break;
    case 4: // left bumper
        btnLBumper = val;
        break;
    case 5: // right bumper
        btnRBumper = val;
        break;
    case 6:
        btn6 = val;
        break;
    case 7:
        btn7 = val;
        break;
//    default:
//        break;
    }
}

void XBoxOne::printALL()
{
    std::cout << "UP\t"    << btnUP   << '\n';
    std::cout << "DOWN\t"  << btnDOWN << '\n';
    std::cout << "LEFT\t"  << btnLEFT << '\n';
    std::cout << "RIGHT\t" << btnRIGHT << '\n';
    std::cout << "A\t" << btnA << '\n';
    std::cout << "B\t" << btnB << '\n';
    std::cout << "Y\t" << btnY << '\n';
    std::cout << "X\t" << btnX << '\n';
    std::cout << "6\t" << btn6 << '\n';
    std::cout << "7\t" << btn7 << '\n';
    std::cout << "lBumper\t"  << btnLBumper << '\n';
    std::cout << "rBumper\t"  << btnRBumper << '\n';
    std::cout << "Left x\t"   << L_x() << '\n';
    std::cout << "Left y\t"   << L_y() << '\n';
    std::cout << "Right x\t"  << R_x() << '\n';
    std::cout << "Right y\t"  << R_y() << '\n';
    std::cout << "lTrigger\t" << joyLTrigger << '\n';
    std::cout << "rTrigger\t" << joyRTrigger << std::endl;
}
