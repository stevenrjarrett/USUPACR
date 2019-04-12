//*******Header*******

 #ifndef MAINWINDOW_H
 #define MAINWINDOW_H

 //#include <QMainWindow>
 //#include <QPushButton>
 //#include <QRadioButton>
 //#include <QGroupBox>

// #include <QWidget>
 #include <QtGui>

 class QGroupBox;

 class MainWindow : public QWidget
 {
    Q_OBJECT

 public:
    MainWindow(QWidget *parent = 0);

 private slots:
    void handleButton();

 private:
    QGroupBox *createDistanceGroup();
    QGroupBox *createSpeedGroup();
    QGroupBox *createAutonomousGroup();
 };

 #endif
