#include "Filter.h"
#include "ImageProcessing.h"

/// Setting the mode of execution (programmatically)
#ifdef GRAPHICAL
    int mode = 0;
#else
    int mode = 1;
#endif

using namespace cv;
using namespace std;

int main(int, char**) {

    int i,j;
    Vec3f pixel;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    namedWindow("Image Processing",1);
    namedWindow("Image Processing1",1);
    moveWindow("Image Processing1", WIDTH, 0); 
  
    cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //Window's size
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); // 320*240 at most


    MatIterator_<Vec3b> it, end;

    int blueTresh = 100;
    double tresh = 0.7;
    //Main loop
    while(1){
        if(cap.read(frame)){// get a new frame from camera    
            int xMin = 0, xMax = 0, yMin = 0, yMax = 0;
	        imshow("Image Processing", frame);	    
            //Getting the blue zones
            getBlueImage(&frame, blueTresh, tresh);
            
            int x, y;
            getGravityCenter(&frame, &x, &y);

            //Drawing the gravity center
            Vec3b pixel(255, 255, 255);
            frame.at<Vec3b>(Point(y, x)) = pixel;
            frame.at<Vec3b>(Point(y-1, x)) = pixel;
            frame.at<Vec3b>(Point(y+1, x)) = pixel;
            frame.at<Vec3b>(Point(y, x+1)) = pixel;
            frame.at<Vec3b>(Point(y, x-1)) = pixel;
            
            cout << x << " " << y << endl;
	        imshow("Image Processing1", frame);	    
        }

        if(waitKey(30) >= 0) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
