///////////////////////////////////////////////////////////////////
//                                                               //
//                   Utah State University                       //
//              Personal Assist Cargo Relay (PACR)               //
//                        Final Code                             //
//                           2019                                //
//                                                               //
//                                                               //
// This software is public domain, and may be copied and used    //
// free of charge by any party.                                  //
//                                                               //
// Note that this file does not contain the primary control and  //
// communication functions for the PACR. That code is contained  //
// in "../src/mainwindow.cpp" in order to prevent collisions     //
// between the QT and librealsense2 libraries.                   //
//                                                               //
///////////////////////////////////////////////////////////////////



#include "mainwindow.h"



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




