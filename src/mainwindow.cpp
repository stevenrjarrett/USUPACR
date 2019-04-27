//*******MainWindow*******

// Contains the class for the main GUI window
//
// Also contains the main program code below the class declaration.

 #include "main.h"
 #include "mainwindow.h"

 #include <QWidget>
 #include <QAbstractButton>

std::fstream motorArduino;

 MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
 {
    // initialize variables
    autonomousEngaged = false;
    //create grid for group boxes
    QGridLayout *grid = new QGridLayout;
    distanceGroup   = createDistanceGroup();
    speedGroup      = createSpeedGroup();
    autonomousGroup = createAutonomousGroup();
    grid->addWidget(distanceGroup, 0, 0);
    grid->addWidget(speedGroup, 1, 0);
    grid->addWidget(autonomousGroup, 0, 1);
    setLayout(grid);

    // Start main program
    mainThread = std::thread(mainProgram, this);

    //resize(XXX,XXX);
 }

 MainWindow::~MainWindow()
 {
//    delete QGridLayout;
//    delete autonomousGroup;
//    delete distanceGroup;
//    delete speedGroup;
//    delete pushButton;
    stop_signal_recieved = true;
    mainThread.join();
 }

 QGroupBox *MainWindow::createDistanceGroup()
 {
    // Create the group of radial buttons for distance
    QGroupBox *groupBox1 = new QGroupBox(tr("Distance"));

    distRadios[0] = new QRadioButton(tr("&1.5 m"));
    distRadios[1] = new QRadioButton(tr("&3 m"));
    distRadios[2] = new QRadioButton(tr("&4.5 m"));
    distRadios[3] = new QRadioButton(tr("&6 m"));

    distRadios[3]->setChecked(true);

    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(distRadios[0]);
    vbox1->addWidget(distRadios[1]);
    vbox1->addWidget(distRadios[2]);
    vbox1->addWidget(distRadios[3]);
    vbox1->addStretch(1);
    groupBox1->setLayout(vbox1);

    return groupBox1;
 }

 QGroupBox *MainWindow::createSpeedGroup()
 {
    // Create the group of radial buttons for speed
    QGroupBox *groupBox2 = new QGroupBox(tr("Speed"));

    speedRadios[0] = new QRadioButton(tr("&Max Speed"));
    speedRadios[1] = new QRadioButton(tr("&3/4 Speed"));
    speedRadios[2] = new QRadioButton(tr("&2/4 Speed"));
    speedRadios[3] = new QRadioButton(tr("&1/4 Speed"));

    speedRadios[0]->setChecked(true);

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(speedRadios[0]);
    vbox2->addWidget(speedRadios[1]);
    vbox2->addWidget(speedRadios[2]);
    vbox2->addWidget(speedRadios[3]);
    vbox2->addStretch(1);
    groupBox2->setLayout(vbox2);

    return groupBox2;
 }

 QGroupBox *MainWindow::createAutonomousGroup()
 {
    // Create the button
    QGroupBox *groupBox3 = new QGroupBox(tr("Autonomous Mode"));

    pushButton = new QPushButton(tr("&OFF"));
    pushButton->setCheckable(true);
    pushButton->setChecked(true);

    // Connect button signal to appropriate slot
    connect(pushButton, SIGNAL (clicked()), this, SLOT (handleButton()));

    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(pushButton);
    vbox3->addStretch(1);
    groupBox3->setLayout(vbox3);

    return groupBox3;
 }


double MainWindow::getMaxSpeed()
{
    if(speedRadios[0]->isChecked())
        return 1.0;
    if(speedRadios[1]->isChecked())
        return .75;
    if(speedRadios[2]->isChecked())
        return .5;
    if(speedRadios[3]->isChecked())
        return .25;
    return 0.0;
}
double MainWindow::getFollowDistance()
{
    if(distRadios[0]->isChecked())
        return 1.5;
    if(distRadios[1]->isChecked())
        return 3.0;
    if(distRadios[2]->isChecked())
        return 4.5;
    if(distRadios[3]->isChecked())
        return 6.0;
    return 0.0;
}

 void MainWindow::handleButton()
 {
    pushButton->setText("ON");
    //activate autonomous mode
    autonomousEngaged = true;

    connect(pushButton, SIGNAL (clicked()), this, SLOT (handleButton2()));

 }

 void MainWindow::handleButton2()
 {
    pushButton->setText("OFF");
    //deactivate autonomous mode
    autonomousEngaged = false;

    connect(pushButton, SIGNAL (clicked()), this, SLOT (handleButton()));
 }





/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Functions //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


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

void sendMotorValues()
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

void motorUpdator()
{
    int serialPortNumber = 0;
    std::string serialPortPath;
    std::string msg;
    std::cout << "Opening serial connection with motor arduino" << std::endl;
    for( ; !(motorArduino.is_open()); serialPortNumber = (serialPortNumber + 1) % 20)
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
            for( ; !(motorArduino.is_open()); serialPortNumber = (serialPortNumber + 1) % 20)
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





/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////  Main  ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//void mainProgram(QApplication *GUI_app, MainWindow *GUI);


int mainProgram(MainWindow *GUI)
{
    bool wasActive = false;

    std::cout << "USU PACR Control Program" << std::endl;
    usleep(10000); // wait to make sure the main window is set up.

    /// Initialize
        // Joystick
        XBoxOne controller;
        controller.setTimeout(controllerTimeout);
        // Cameras/autonomous function
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
        motor_speed_limiter = GUI->getMaxSpeed();
        follow_distance     = GUI->getFollowDistance();
//        bool autonomous_mode_new = GUI->getAutonomousEngaged();
//        if(autonomous_mode != autonomous_mode_new)
//        {
//            motors_target.left = 0;
//            motors_target.right = 0;
//            autonomous_mode = autonomous_mode_new;
//        }
//        std::cout << "maxSpeed = "         <<  motor_speed_limiter
//                  << ", followDistance = " <<  follow_distance
//                  << ", autonomousMode = " <<  autonomous_mode     << std::endl;
//        std::cout << "Beginning of executive loop" << std::endl;
        // Get closing signal from GUI
//        if(GUI_app.lastWindowClosed())
//        {
//            stop_signal_recieved = true;
//            break;
//        }
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
//        if(controller.wp_lBumper())
//        {
//            motors_target.left = 0;
//            motors_target.right = 0;
////            sendMotorValues();
//            break;
//        }
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
//                if(controller.wp_B() && (GUI->getAutonomousEngaged()))
//                {
//                    GUI->pushButton->animateClick();
//                }
                if(!(GUI->getAutonomousEngaged()))
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
                    double turningVal = 0;
                    double speedVal   = 0; // positive for forward, negative for backward
                    double brakeVal   = 0; // 0 for no brakes, 1 for all brakes


                    if(tracker.getCentroid().z >= (follow_distance - distance_tolerance))
                    {
                        //Turning
                        double turningVal = (double)tracker.getCentroid().x * autonomous_max_speed / autonomous_x_tolerance; // positive to turn right, negative to turn left.
                        if(abs_val(turningVal) > autonomous_max_speed)
                            turningVal = turningVal / abs_val(turningVal) * autonomous_max_speed;


                        // Speed:
                        speedVal = (tracker.getCentroid().z - follow_distance + distance_tolerance) / (2*distance_tolerance);
                        if(speedVal > 1.0)
                            speedVal = 1.0;
                        if(speedVal < 0.0)
                            speedVal = 0.0;
                        // speedVal should be in a value between 0 and 1 if the person is at the target following
                        // distance +- the distance tolerance.
                        if(speedVal > 0.25)
                        {
                            brakeVal = 0;
                            speedVal -= 0.25;
                            speedVal *= 1.0/0.75;
                            speedVal *= max_speed;
                        }
                        else
                        {
                            brakeVal = (0.25 - speedVal) * (1.0 / 0.25);
                            speedVal = 0;
                        }

                        // If the person's farther away, fix the speed value.
                    }
                    motors_target = ConvertToArcade(turningVal, speedVal);
                    motors_target.brake = brakeVal;
                }
                else
                {
                    motors_target.left = 0;
                    motors_target.right = 0;
                    motors_target.brake = 0;
                }
            }
            else // manual control
            {
//                if(controller.wp_A() && !(GUI->getAutonomousEngaged()))
//                {
//                    GUI->pushButton->animateClick();
//                }
                if((GUI->getAutonomousEngaged()))
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
                    motors_target.brake = (controller.lTrigger() + 1.0) / 2.0;
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
//    bool hasGuiApp = (qobject_cast<QApplication*>(QCoreApplication::instance())!=0);
//    if(GUI_app.instance()!=0)
//        GUI_app->quit();

//    return 0;


//    std::exit(1);
}












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

