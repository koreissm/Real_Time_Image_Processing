#include "ImageProcessing.h"
#include "Bot.h"
#include <sys/time.h>
#include<vector>
#include "SerialClass.h"

#define BLUE_TRESH 100
#define TRESH 0.7
#define NEIGHBORHOOD 15
#define TASK_PERIOD 1000 //Micro-seconde

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
    Arduino arduino;
    arduino.filename = DEVICE_NAME;
    arduino.fd = serialport_init(DEVICE_NAME, 9600);
    const int buf_max = 256;
    char buf[buf_max];

    int x1, y1;
    int xImg = WIDTH/2, yImg = HEIGHT/2;
    int xMove = 90, yMove = 180;
    
    struct timeval start,checkpoint;
    long long diff;
    gettimeofday(&start, 0);

    //Main loop
    while(1){
        gettimeofday(&checkpoint, 0);
        diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec);
         
        if (diff < TASK_PERIOD ) ; // Execution toutes les TASK_PERIOD
        else {
            gettimeofday(&start, 0);

            if(cap.read(frame)){// get a new frame from camera    
                images.push_back(frame); //Adding the new image to the array

                int xMin = 0, xMax = 0, yMin = 0, yMax = 0;
	            imshow("Image Processing", frame);	    
                //Getting the blue zones
                getBlueImage(&frame, BLUE_TRESH, TRESH);
            
                getGravityCenter(&frame, &xMin, &xMax, &yMin, &yMax);
                int x = (xMin + xMax)/2;
                int y = (yMin + yMax)/2;            
                //Drawing the gravity center
                circle(frame, Point(x, y), 8, Scalar(0, 0, 255), 2);

            
                //Give some orders to the bot
                int vx = x - xImg, vy = y - yImg;
                if ((vx > NEIGHBORHOOD) && (vy < NEIGHBORHOOD && vy > -NEIGHBORHOOD)) {if (xMove < 180) xMove++;}
                else if ((vx > NEIGHBORHOOD) && (vy > NEIGHBORHOOD)) {if (xMove < 180) xMove++; if (yMove < 180) yMove++;}
                else if ((vx > -NEIGHBORHOOD && vx < NEIGHBORHOOD) && (vy > NEIGHBORHOOD)) {if (yMove < 180) yMove++;}
                else if ((vx < -NEIGHBORHOOD) && (vy > NEIGHBORHOOD)) {if (xMove > 0) xMove--; if (yMove < 180) yMove++;}
                else if ((vx < -NEIGHBORHOOD) && (vy < NEIGHBORHOOD && vy > -NEIGHBORHOOD)) {if (xMove > 0) xMove--;}
                else if ((vx < -NEIGHBORHOOD) && (vy < -NEIGHBORHOOD)) {if (xMove > 0) xMove--; if (yMove > 0) yMove--;}//
                else if ((vx > -NEIGHBORHOOD && vx < NEIGHBORHOOD) && (vy < -NEIGHBORHOOD)) {if (yMove > 0) yMove--;}
                else if ((vx > NEIGHBORHOOD) && (vy < -NEIGHBORHOOD)) {if (xMove < 180) xMove++; if (yMove > 0) yMove--;}
    
                cout << "Xmove : " << xMove << ", Ymove : " << yMove << endl;
                char* string = new char[10];
                sprintf(string,"%d:%d,", xMove, yMove); //Writing to the file
                serialport_write(arduino.fd, string);    

                //cout << "xMin : " << xMin << " , yMin : " << yMin << ", xMax : " << xMax << ", yMax : " << yMax << endl;
//              cout << "x1 : " << x1 << ", y1 : " << y1 << ", x2 : " << x2 << ", y2 : " << y2 << endl;
	            imshow("Image Processing1", frame);	    

            }

            if(waitKey(5) >= 0) break;
        }
    }
    serialport_close(arduino.fd);

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
