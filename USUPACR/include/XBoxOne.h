#ifndef XBOXONE_H
#define XBOXONE_H

#include <thread>
#include "../libenjoy/src/libenjoy.h"
#include "../libenjoy/src/libenjoy_linux.h"


class XBoxOne
{
public:
        XBoxOne();
        ~XBoxOne();

        void start();
        void stop();
        bool isConnected();
protected:

private:
    void run();
    void setBtn(int id, bool val);
    void setAxis(int id, int val);
    bool isRunning,
         connected;
    std::thread pollingThread;
    libenjoy_context *ctx; // initialize the library
    libenjoy_joy_info_list *info;
    bool btnUP,
         btnDOWN,
         btnLEFT,
         btnRIGHT,
         btnA,
         btnB,
         btnX,
         btnY,
         btnLBumper,
         btnRBumper;
    int joyL_x,
        joyL_y,
        joyR_x,
        joyR_y;
};

#endif // XBOXONE_H
