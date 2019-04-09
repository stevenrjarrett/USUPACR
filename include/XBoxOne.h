#ifndef XBOXONE_H
#define XBOXONE_H

#include <thread>
#include "libenjoy.h"
#include "libenjoy_linux.h"
#include "stopwatch.h"

#define XJOY_MAX 32768


class XBoxOne
{
public:
        XBoxOne();
        ~XBoxOne();

        // control functions
        void start();
        void stop();

        // Class information accessors
        bool isConnected();
        bool isRunning(){ return running; }
        bool isActive(){ return active; }

        // Print / debug functions
        void printALL();

        // Button value accessors
        bool UP(){return btnUP;}
        bool DOWN(){return btnDOWN;}
        bool LEFT(){return btnLEFT;}
        bool RIGHT(){return btnRIGHT;}
        bool A(){return btnA;}
        bool B(){return btnB;}
        bool X(){return btnX;}
        bool Y(){return btnY;}
        bool lBumper(){return btnLBumper;}
        bool rBumper(){return btnRBumper;}
        double L_x(){return (double)joyL_x/XJOY_MAX;}
        double L_y(){return (double)joyL_y/XJOY_MAX;}
        double R_x(){return (double)joyR_x/XJOY_MAX;}
        double R_y(){return (double)joyR_y/XJOY_MAX;}
        double lTrigger(){return (double)joyLTrigger/XJOY_MAX;}
        double rTrigger(){return (double)joyRTrigger/XJOY_MAX;}

        // "was pressed" button accessors
protected:

private:
    /// Activity Checker
    bool active;
    std::thread activityThread;
    stopwatch activityStopwatch;
    void activityChecker();

    /// Run function and utilities
    bool running,
         connected;
    void run();
    void setBtn(int id, int val);
    void setAxis(int id, int val);
    std::thread pollingThread;

    /// libenjoy
    libenjoy_context *ctx; // initialize the library
    libenjoy_joy_info_list *info;

    /// Button Values
    bool btnUP,
         btnDOWN,
         btnLEFT,
         btnRIGHT,
         btnA,
         btnB,
         btnX,
         btnY,
         btnLBumper,
         btnRBumper,
         btn6,
         btn7;
    int joyL_x,
        joyL_y,
        joyR_x,
        joyR_y,
        joyLTrigger,
        joyRTrigger;
};

#endif // XBOXONE_H
