#include "motorController.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

motorController::motorController()
{
    //ctor
}

motorController::~motorController()
{
    //dtor
}



//using namespace std;

// function... might want it in some class?
//int motorController::getdir (std::string dir, std::vector<string> &files, std::string filter = "")
//{
//    DIR *dp;
//    struct dirent *dirp;
//    if((dp  = opendir(dir.c_str())) == NULL) {
//        cout << "Error(" << errno << ") opening " << dir << endl;
//        return errno;
//    }
//
//    while ((dirp = readdir(dp)) != NULL) {
//        if (s1.find(s2) != std::string::npos)
//        {
//            files.push_back(std::string(dirp->d_name));
//        }
//    }
//    closedir(dp);
//    return 0;
//}
//
//int main()
//{
//    string dir = string(".");
//    vector<string> files = vector<string>();
//
//    getdir(dir,files);
//
//    for (unsigned int i = 0;i < files.size();i++) {
//        cout << files[i] << endl;
//    }
//    return 0;
//}
