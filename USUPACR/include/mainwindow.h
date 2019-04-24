//*******Header*******

 #ifndef MAINWINDOW_H
 #define MAINWINDOW_H

 #include <QtGui>

 class QGroupBox;

 class MainWindow : public QWidget
 {
    Q_OBJECT

 public:
    MainWindow(QWidget *parent = 0);

 private slots:
    void handleButton();
    void handleButton2();

 private:
    QGroupBox *createDistanceGroup();
    QGroupBox *createSpeedGroup();
    QGroupBox *createAutonomousGroup();
    QPushButton *pushButton;
 };

 #endif