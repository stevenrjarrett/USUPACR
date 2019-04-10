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

        // Setters
        void setTimeout(double _time){if(_time>0) activity_timeout = _time;}

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
        // Button value accessors
        bool wp_UP(){bool tmp = wp_btnUP; wp_btnUP = false; return tmp;}
        bool wp_DOWN(){bool tmp = wp_btnDOWN; wp_btnDOWN = false; return tmp;}
        bool wp_LEFT(){bool tmp = wp_btnLEFT; wp_btnLEFT = false; return tmp;}
        bool wp_RIGHT(){bool tmp = wp_btnRIGHT; wp_btnRIGHT = false; return tmp;}
        bool wp_A(){bool tmp = wp_btnA; wp_btnA = false; return tmp;}
        bool wp_B(){bool tmp = wp_btnB; wp_btnB = false; return tmp;}
        bool wp_X(){bool tmp = wp_btnX; wp_btnX = false; return tmp;}
        bool wp_Y(){bool tmp = wp_btnY; wp_btnY = false; return tmp;}
        bool wp_lBumper(){bool tmp = wp_btnLBumper; wp_btnLBumper = false; return tmp;}
        bool wp_rBumper(){bool tmp = wp_btnRBumper; wp_btnRBumper = false; return tmp;}
        double wp_lTrigger(){bool tmp = wp_joyLTrigger; wp_joyLTrigger = false; return tmp;}
        double wp_rTrigger(){bool tmp = wp_joyRTrigger; wp_joyRTrigger = false; return tmp;}
protected:

private:
    /// Activity Checker
    bool active;
    std::thread activityThread;
    stopwatch activityStopwatch;
    double activity_timeout; // number of seconds the controller is inactive before the class marks it as inactive
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
    // button "was-pressed" values
    bool wp_btnUP,
         wp_btnDOWN,
         wp_btnLEFT,
         wp_btnRIGHT,
         wp_btnA,
         wp_btnB,
         wp_btnX,
         wp_btnY,
         wp_btnLBumper,
         wp_btnRBumper,
         wp_btn6,
         wp_btn7,
         wp_joyLTrigger,
         wp_joyRTrigger;

};

#endif // XBOXONE_H
