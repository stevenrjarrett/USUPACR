//*******MainWindow*******

 #include "mainwindow.h"

 #include <QWidget>
 #include <QAbstractButton>

 MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
 {
    //create grid for group boxes
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createDistanceGroup(), 0, 0);
    grid->addWidget(createSpeedGroup(), 1, 0);
    grid->addWidget(createAutonomousGroup(), 0, 1);
    setLayout(grid);

    //resize(XXX,XXX);
 }

 QGroupBox *MainWindow::createDistanceGroup()
 {
    // Create the group of radial buttons for distance
    QGroupBox *groupBox1 = new QGroupBox(tr("Distance"));

    QRadioButton *radio1 = new QRadioButton(tr("&1.5 m"));
    QRadioButton *radio2 = new QRadioButton(tr("&3 m"));
    QRadioButton *radio3 = new QRadioButton(tr("&4.5 m"));
    QRadioButton *radio4 = new QRadioButton(tr("&6 m"));

    radio1->setChecked(true);

    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(radio1);
    vbox1->addWidget(radio2);
    vbox1->addWidget(radio3);
    vbox1->addWidget(radio4);
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

    //set size for autonomous button
    //pushButton->resize(100,100);

    // Connect button signal to appropriate slot
    connect(pushButton, SIGNAL (clicked()), this, SLOT (handleButton()));

    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(pushButton);
    vbox3->addStretch(1);
    groupBox3->setLayout(vbox3);

    return groupBox3;
 }


 void MainWindow::handleButton()
 {
    pushButton->setText("ON");
 }
