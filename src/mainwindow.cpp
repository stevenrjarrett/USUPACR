//*******MainWindow*******

 #include "mainwindow.h"

 #include <QWidget>
 #include <QAbstractButton>

 MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
 {
    //create grid for group boxes
    QGridLayout *grid = new QGridLayout;
    distanceGroup   = createDistanceGroup();
    speedGroup      = createSpeedGroup();
    autonomousGroup = createAutonomousGroup();
    grid->addWidget(distanceGroup, 0, 0);
    grid->addWidget(speedGroup, 1, 0);
    grid->addWidget(autonomousGroup, 0, 1);
    setLayout(grid);

    //resize(XXX,XXX);
 }

 MainWindow::~MainWindow()
 {
//    delete QGridLayout;
//    delete autonomousGroup;
//    delete distanceGroup;
//    delete speedGroup;
//    delete pushButton;
 }

 QGroupBox *MainWindow::createDistanceGroup()
 {
    // Create the group of radial buttons for distance
    QGroupBox *groupBox1 = new QGroupBox(tr("Distance"));

    distRadios[0] = new QRadioButton(tr("&1.5 m"));
    distRadios[1] = new QRadioButton(tr("&3 m"));
    distRadios[2] = new QRadioButton(tr("&4.5 m"));
    distRadios[3] = new QRadioButton(tr("&6 m"));

    distRadios[0]->setChecked(true);

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

    QRadioButton *radio5 = new QRadioButton(tr("&5 kph"));
    QRadioButton *radio6 = new QRadioButton(tr("&10 kph"));
    QRadioButton *radio7 = new QRadioButton(tr("&15 kph"));
    QRadioButton *radio8 = new QRadioButton(tr("&20 kph"));

    radio5->setChecked(true);

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(radio5);
    vbox2->addWidget(radio6);
    vbox2->addWidget(radio7);
    vbox2->addWidget(radio8);
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
    return 1.0;
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
