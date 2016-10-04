#include "ImageProcessing.h"
#include "Bot.h"
#include<vector>

#define BLUE_TRESH 100
#define TRESH 0.7


/// Setting the mode of execution (programmatically)
#ifdef GRAPHICAL
    int mode = 0;
#else
    int mode = 1;
#endif

using namespace cv;
using namespace std;

vector<Mat> images;

int main(int, char**) {

    int i,j;

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

    int x1, y1;
    //Main loop
    while(1){
        if(cap.read(frame)){// get a new frame from camera    
            images.push_back(frame); //Adding the new image to the array

            int xMin = 0, xMax = 0, yMin = 0, yMax = 0;
	        imshow("Image Processing", frame);	    
            //Getting the blue zones
            getBlueImage(&frame, BLUE_TRESH, TRESH);
            
            getGravityCenter(&frame, &xMin, &xMax, &yMin, &yMax);
            int x2 = (xMin + xMax)/2;
            int y2 = (yMin + yMax)/2;            
            //Drawing the gravity center
            circle(frame, Point(x2, y2), 8, Scalar(0, 255, 0), 2);

            
            //Give some orders to the bot
            giveSomeOrders(x1, y1, x2, y2);

            cout << "xMin : " << xMin << " , yMin : " << yMin << ", xMax : " << xMax << ", yMax : " << yMax << endl;
            cout << "x2 : " << x2 << ", y2 : " << y2 << endl;
	        imshow("Image Processing1", frame);	    

            x1 = x2; y1 = y2;
        }

        if(waitKey(30) >= 0) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
