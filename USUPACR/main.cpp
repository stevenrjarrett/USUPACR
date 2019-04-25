#include "mainwindow.h"

std::fstream motorArduino;


int main(int argc, char** argv)
{
    QApplication GUI_app(argc, argv);
     MainWindow GUI;
     GUI.showMaximized();


//    //Run GUI
////         std::thread guiThread = std::thread(GUI_app.exec, &GUI_app);
////    int returnMsg = GUI_app.exec();
//    int returnMsg = 1;
//    GUI_app.exec();
//
//    // stop main program
    return GUI_app.exec();
}




