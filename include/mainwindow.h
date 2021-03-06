// MainWindow is a Qt based class that runs the GUI for the PACR. It will also start the main control program.

//*******Header*******

 #ifndef MAINWINDOW_H
 #define MAINWINDOW_H

// #include "main.h"
 #include <QtGui>
 #include <thread>

 #define NUM_DISTANCE_RADIOS 4
 #define NUM_SPEED_RADIOS    4

 class QGroupBox;

 class MainWindow : public QWidget
 {
    Q_OBJECT

 public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool getAutonomousEngaged() { return autonomousEngaged; }
    double getMaxSpeed();
    double getFollowDistance();
    QPushButton *pushButton;

 private slots:
    void handleButton();
    void handleButton2();

 private:
    QGroupBox *createDistanceGroup();
    QGroupBox *createSpeedGroup();
    QGroupBox *createAutonomousGroup();

    // variables
    QGroupBox *autonomousGroup;
    QGroupBox *distanceGroup;
    QGroupBox *speedGroup;

    QRadioButton *distRadios[NUM_DISTANCE_RADIOS];
    QRadioButton *speedRadios[NUM_SPEED_RADIOS];

    bool autonomousEngaged;
    double maxSpeed;
    double followDistance;

    // main program
    std::thread mainThread;

 };

 #endif
