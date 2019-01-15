#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <cmath>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[4];

    Ptr<Tracker> tracker;

    #if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
//        if (trackerType == "MOSSE")
//            tracker = TrackerMOSSE::create();
//        if (trackerType == "CSRT")
//            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video(1);

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame
    Mat frame;
    bool ok = video.read(frame);

    // Define initial bounding box
    Rect2d bbox(3*frame.cols/8, frame.rows/4, frame.cols/4, frame.rows/2);
    while(true)
    {
    //    Rect2d bbox(frame.rows/4, frame.cols/4, 3*frame.rows/4, 3*frame.cols/4);

        // Uncomment the line below to select a different bounding box
        // bbox = selectROI(frame, false);
        // Display bounding box.
        rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        imshow("Tracking", frame);

        char c = cv::waitKey(30);
        if(c=='\n' || c==13 || c==27)
            break;

        video.read(frame);

    }

    tracker->init(frame, bbox);

    double angle = 0.0;

    while(video.read(frame))
    {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );

            // calculate angle
            double xcntr = bbox.x+bbox.width/2;
            double x = xcntr - (frame.cols/2);
            angle = 180.0/3.1415926*atan(x/((double)frame.cols/2)*tan(3.1415926/6));//x/((double)frame.cols/2) * 30.0;
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);

        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);

        // Display angle on frame
        putText(frame, "Angle : " + SSTR(angle), Point(100,70), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);

        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
}
