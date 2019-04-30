#ifndef PACRCONTROL_H
#define PACRCONTROL_H

#include <thread>

struct motorValues
{
    double left;
    double right;
    double brake;
    bool autonomous;
    motorValues(double lm = 0, double rm = 0, double _brake = 0, bool _autonomous = false)
    {
        left = lm;
        right = rm;
        brake = _brake;
        autonomous = _autonomous;
    }
    motorValues& operator=(const motorValues& rhs)
    {
        left = rhs.left;
        right = rhs.right;
        brake = rhs.brake;
        autonomous = rhs.autonomous;
        return *this;
    }
    bool operator==(const motorValues& rhs)
    {
        return (left == rhs.left) &&
               (right == rhs.right);
    }
    bool operator!=(const motorValues& rhs)
    {
        return (left != rhs.left) ||
               (right != rhs.right);
    }
    motorValues& operator-=(const motorValues& rhs)
    {
        left -= rhs.left;
        right -= rhs.right;
        return *this;
    }
    motorValues& operator+=(const motorValues& rhs)
    {
        left += rhs.left;
        right += rhs.right;
        return *this;
    }
};


class pacrControl
{
    public:
        pacrControl();
        virtual ~pacrControl();
        motorValues motors_target;

    protected:

    private:
        motorValues motors_actual;
        double max_speed; // 255 is the max value you can send. Set this lower for slower
        double max_acceleration; // 1 is for 0 to max_speed in 1 second;
        double autonomous_max_speed; // 255 is the max value you can send. Set this lower for slower
        double motor_speed_limiter; // a value from 0-1, setting the maximum speed.
        bool enable_soft_start;
        double drive_increment;


        void updateMotorValues();
        void sendMotorValues();
        void motorUpdator();

        std::thread runningThread;
        bool stop_signal_recieved;


};

#endif // PACRCONTROL_H
