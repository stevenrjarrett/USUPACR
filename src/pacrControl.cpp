#include "pacrControl.h"



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





pacrControl::pacrControl(bool _enable_soft_start = true, double _max_acceleration = 0.8)
  : stop_signal_recieved(false),
    max_speed(255),
    max_acceleration(0.8),
    autonomous_max_speed(200),
    motor_speed_limiter(1.0),
    enable_soft_start(true),
    drive_increment(max_speed * max_acceleration * ((double)delayTime / 1000000.0)),
    delayTime(200000)
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
    actual.brake = target.brake;
    actual.autonomous = target.autonomous;


    //use soft-start to set motors
    motorValues mtrDiff = target,
                aMtrDiff;
    mtrDiff -= actual;
    aMtrDiff.left  = abs_val(mtrDiff.left );
    aMtrDiff.right = abs_val(mtrDiff.right);
    aMtrDiff.brake = abs_val(mtrDiff.brake);

    bool accelerating = (  (mtrDiff.left  > 0 && target.left  > 0) || (mtrDiff.left  < 0 && target.left  < 0)
                        || (mtrDiff.right > 0 && target.right > 0) || (mtrDiff.right < 0 && target.right < 0) );


//Drive
    if ( accelerating
         && (aMtrDiff.left > drive_increment || aMtrDiff.right > drive_increment )
         && enable_soft_start)
    {
        double max_aDiff = std::max(aMtrDiff.left, aMtrDiff.right);

        actual.left += ( drive_increment * mtrDiff.left) / max_aDiff;
        actual.right += ( drive_increment * mtrDiff.right) / max_aDiff;
    }
    else
    {
        actual.left = target.left;
        actual.right = target.right;
    }
//    std::cout << "ltarget = " << target.left << ", lActual = " << actual.left << ", mtrDiff = " << mtrDiff.left << ", aMtrDiff = " << aMtrDiff.left << std::endl;
}

void pacrControl::sendMotorValues()
{
    motorArduino << std::setw(10) << (int)(actual.left  * motor_speed_limiter) << std::endl;
    motorArduino << std::setw(10) << (int)(actual.right * motor_speed_limiter) << std::endl;
//    motorArduino << (int)actual.autonomous << std::endl;
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
        usleep(delayTime);
    }


}
