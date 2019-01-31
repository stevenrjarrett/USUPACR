#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <cmath>

using namespace cv;
using namespace std;

// Macros

#define PI 3.141592653589
#define CDIST 0.30             // Center distance between cameras

// Header

cv::Rect2d MatchImage( cv::Mat& img, cv::Mat& templ, const int match_method = TM_SQDIFF);

// Global Variables

bool use_mask = false;

// Custom data types

struct coordinate
{
    double x,y,z;
    coordinate(double _x=0, double _y=0, double _z=0):
        x(_x),
        y(_y),
        z(_z)
    {}
};

void bboxFix( const cv::Mat& img, cv::Rect2d& box)
{
    if(box.x < 0)
    {
        box.width += box.x;
        box.x = 0;
    }
    if(box.y < 0)
    {
        box.height += box.y;
        box.y = 0;
    }
    if(box.x+box.width >= img.cols)
    {
        box.width = img.cols-1-box.x;
    }
    if(box.y+box.height >= img.rows)
    {
        box.height = img.rows-1-box.y;
    }
}

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[6];

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
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video(1);
    VideoCapture video2(2);

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first lframe
    Mat lframe, rframe, templ;
    bool ok = video.read(lframe);
//    bool ok2 = video2.read(rframe);

    // Define initial bounding box
    Rect2d lbbox(3*lframe.cols/8, lframe.rows/4, lframe.cols/4, lframe.rows/2);
    Rect2d rbbox;
    while(true)
    {
    //    Rect2d lbbox(lframe.rows/4, lframe.cols/4, 3*lframe.rows/4, 3*lframe.cols/4);

        // Uncomment the line below to select a different bounding box
        // lbbox = selectROI(lframe, false);
        // Display bounding box.
        rectangle(lframe, lbbox, Scalar( 255, 0, 0 ), 2, 1 );
        imshow("Tracking", lframe);

        char c = cv::waitKey(30);
        if(c=='\n' || c==13 || c==27)
            break;

        video.read(lframe);

    }

    tracker->init(lframe, lbbox);

    double al = 0.0,
           ar = 0.0;
    coordinate person;
    while(video.read(lframe) && video2.read(rframe))
    {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(lframe, lbbox);
        bboxFix(lframe, lbbox);

        // Match the image
        if(ok)
        {
//            try
//            {
                templ = lframe(lbbox);
                rbbox = MatchImage(rframe, templ);
//            }
//            catch(...)
//            {
//                std::cout << "Ok, so here's the bbox:\n"
//                          << "  x = " << lbbox.x << "\n"
//                          << "  y = " << lbbox.y << "\n"
//                          << "  w = " << lbbox.width << "\n"
//                          << "  h = " << lbbox.height << std::endl;
//            }
        }

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(lframe, lbbox, Scalar( 255, 0, 0 ), 2, 1 );
            rectangle(rframe, rbbox, Scalar( 255, 0, 0 ), 2, 1 );

            // calculate distance
                // Angle 1
                double lxcntr = lbbox.x+lbbox.width/2;
                double lx = lxcntr - (lframe.cols/2);
                double langle = atan(lx/((double)lframe.cols/2)*tan(3.1415926/6));//x/((double)lframe.cols/2) * 30.0;
                al = PI/2 - langle; // angle to be used for calculations
                // Angle 2
                double rxcntr = rbbox.x+rbbox.width/2;
                double rx = rxcntr - (rframe.cols/2);
                double rangle = atan(rx/((double)rframe.cols/2)*tan(3.1415926/6));//x/((double)lframe.cols/2) * 30.0;
                ar = PI/2 + rangle;
                //
                double a3 = PI - al - ar;          //3rd angle of camera-camera-person triangle
                double d1 = CDIST*(sin(ar)/sin(a3));  // distance from left camera to person
                person.y = d1*sin(al);
                person.x = d1*cos(al) - CDIST/2;

//                std::cout << "a3 = " << a3 << '\n'
//                          << "d1 = " << d1 << '\n'
//                          << "x  = " << person.x << '\n'
//                          << "y  = " << person.y << "\n\n";

        }
        else
        {
            // Tracking failure detected.
            putText(lframe, "Tracking failure detected", Point(10,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }

        // Display tracker type on lframe
        putText(lframe, trackerType + " Tracker", Point(10,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255),2);

        // Display FPS on lframe
        putText(lframe, "FPS : " + SSTR(int(fps)), Point(10,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255), 2);

        // Display langle on lframe
        putText(lframe, "Angle : " + SSTR(180.0/PI*al), Point(10,70), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255), 2);
        putText(rframe, "Angle : " + SSTR(180.0/PI*ar), Point(10,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255), 2);

        //Display person's position on frame
        putText(rframe, "Distance to person:   " + SSTR(person.y), Point(10,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255), 2);
        putText(rframe, "Distance from center: " + SSTR(person.x), Point(10,70), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(100,100,255), 2);


        // Display lframe.
        imshow("Tracking", lframe);
        imshow("Matching", rframe);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
}



/**
  * @MatchImage
  *
*/

cv::Rect2d MatchImage( cv::Mat& img, cv::Mat& templ, const int match_method)
{
  cv::Mat mask;
  cv::Mat result;
  //! [copy_source]
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );
  //! [copy_source]

  //! [create_result_matrix]
  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );
  //! [create_result_matrix]

  //! [match_template]
  /// Do the Matching and Normalize
  bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
  if (use_mask && method_accepts_mask)
    { matchTemplate( img, templ, result, match_method, mask); }
  else
    { matchTemplate( img, templ, result, match_method); }
  //! [match_template]

  //! [normalize]
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
  //! [normalize]

  //! [best_match]
  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  //! [best_match]

  //! [match_loc]
  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  //! [match_loc]

  //! [imshow]
  /// Show me what you got
//  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
//  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
//
//  imshow( image_window, img_display );
//  imshow( result_window, result );
  //! [imshow]

  return cv::Rect2d(matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ));
}
