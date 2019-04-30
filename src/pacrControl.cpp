#include "pacrControl.h"

pacrControl::pacrControl(bool _enable_soft_start = true, double _max_acceleration = 0.8)
  : stop_signal_recieved(false),
    max_speed(255),
    max_acceleration(0.8),
    autonomous_max_speed(200),
    motor_speed_limiter(1.0),
    enable_soft_start(true),
    drive_increment(max_speed * max_acceleration * ((double)MOTORS_WAIT_TIME / 1000000.0))
{
    runningThread = std::thread(motorUpdator, this);
}

pacrControl::~pacrControl()
{
    //dtor
    stop_signal_recieved = true;
    std::cout << "Stopping Motors:" << std::endl;
    if(runningThread.joinable())
        runningThread.join();
    std::cout << "\tDone" << std::endl;

}


void pacrControl::updateMotorValues()
{
    // Set brake
    motors_actual.brake = motors_target.brake;
    motors_actual.autonomous = motors_target.autonomous;


    //use soft-start to set motors
    motorValues mtrDiff = motors_target,
                aMtrDiff;
    mtrDiff -= motors_actual;
    aMtrDiff.left  = abs_val(mtrDiff.left );
    aMtrDiff.right = abs_val(mtrDiff.right);
    aMtrDiff.brake = abs_val(mtrDiff.brake);

    bool accelerating = (  (mtrDiff.left  > 0 && motors_target.left  > 0) || (mtrDiff.left  < 0 && motors_target.left  < 0)
                        || (mtrDiff.right > 0 && motors_target.right > 0) || (mtrDiff.right < 0 && motors_target.right < 0) );


//Drive
    if ( accelerating
         && (aMtrDiff.left > drive_increment || aMtrDiff.right > drive_increment )
         && enable_soft_start)
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
//    std::cout << "ltarget = " << motors_target.left << ", lActual = " << motors_actual.left << ", mtrDiff = " << mtrDiff.left << ", aMtrDiff = " << aMtrDiff.left << std::endl;
}

void pacrControl::sendMotorValues()
{
    motorArduino << std::setw(10) << (int)(motors_actual.left  * motor_speed_limiter) << std::endl;
    motorArduino << std::setw(10) << (int)(motors_actual.right * motor_speed_limiter) << std::endl;
//    motorArduino << (int)(motors_actual.left  * motor_speed_limiter) << '\n'
//                 << (int)(motors_actual.right * motor_speed_limiter) << '\n';
//    motorArduino << (float)motors_actual.brake << std::endl;
//    motors_actual = motors_target;
//    std::cout << "sent values: l=" << (int)(motors_target.left  * motor_speed_limiter)
//                         << ", r=" << (int)(motors_target.right * motor_speed_limiter) << std::endl;
}

void pacrControl::motorUpdator()
{
    int serialPortNumber = 0;
    std::string serialPortPath;
    std::string msg;
    std::cout << "Opening serial connection with motor arduino" << std::endl;
    for( ; !(motorArduino.is_open()) && !stop_signal_recieved; serialPortNumber = (serialPortNumber + 1) % 20)
    {
        serialPortPath = std::string("/dev/ttyACM")+std::to_string(serialPortNumber);
        motorArduino.open(serialPortPath, std::ios_base::out | std::ios_base::in);
        usleep(1000);
    }
    usleep(100000);
    std::cout << "Serial port for motors successfully opened at " << serialPortPath << std::endl;
//    motorArduino.peek();
//    motorArduino >> msg;
    // main loop
    while(!stop_signal_recieved)
    {
        // Check for connection
        if(!(motorArduino.is_open()))
        {
            std::cout << "Lost connection to motor Arduino. Attempting to recover..." << std::endl;
            for( ; !(motorArduino.is_open()) && !stop_signal_recieved; serialPortNumber = (serialPortNumber + 1) % 20)
            {
                serialPortPath = std::string("/dev/ttyACM")+std::to_string(serialPortNumber);
                motorArduino.open(serialPortPath, std::ios_base::out | std::ios_base::in | std::ios_base::trunc );
                usleep(1000);
            }
        }

        // update motor values
        updateMotorValues();
        sendMotorValues();
//        motorArduino >> msg;
//        if(msg != "")
//        {
//            std::cout << "Arduino: " << msg;
//        }
        usleep(MOTORS_WAIT_TIME);
    }


}
