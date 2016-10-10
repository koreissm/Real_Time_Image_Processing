#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <vector>

#define NB_INTEREST_POINTS 30
#define WIDTH 320
#define HEIGHT 240

using namespace std;
using namespace cv;

typedef struct {
    uchar color;
    int x, y;
} Pixel;

bool comparePixels(Pixel p, Pixel q) {
    return p.color < q.color;
}

int main(int, char**) {

    int i,j;
    int g, b;
    Vec3f pixel;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame, gray, sobelX, sobelY;
    namedWindow("MyCam",1);
    namedWindow("Norme",1);
    moveWindow("Norme", 325, 20);
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag

    vector<Mat> images;

    MatIterator_<Vec3b> it, end;

    double k_old[HEIGHT][WIDTH];
    map <double, Pixel> pixels_old;

    int nbFrame = 0;
    while(1){
        if(cap.read(frame)){// get a new frame from camera
            sobelX = Mat(frame.rows, frame.cols, CV_8UC1, Scalar(0, 0, 0)); 
            sobelY = Mat(frame.rows, frame.cols, CV_8UC1, Scalar(0, 0, 0)); 
            cvtColor(frame, gray, CV_BGR2GRAY);
            Mat norme(frame.rows, frame.cols, CV_8UC1, Scalar(0, 0, 0));
            long sumX, sumY;

            //Calculating the gradient
            for(int x = 1; x < gray.rows-1 ; x++) {
                for(int y = 1; y < gray.cols-1 ; y++){
                    sumX = -gray.at<uchar>(x-1, y-1) - 2*gray.at<uchar>(x-1, y) - gray.at<uchar>(x-1, y+1) + gray.at<uchar>(x+1, y-1) + 2*gray.at<uchar>(x+1, y-1) + gray.at<uchar>(x+1, y+1);
                    sumY =  -gray.at<uchar>(x-1, y-1) + gray.at<uchar>(x-1, y+1) - 2*gray.at<uchar>(x, y-1) + 2*gray.at<uchar>(x, y+1) - gray.at<uchar>(x+1, y-1) + gray.at<uchar>(x+1, y+1);
                    sobelX.at<uchar>(x,y) = (uchar) (abs(sumX)/4);
                    sobelY.at<uchar>(x,y) = (uchar) (abs(sumY)/4);
            
                    norme.at<uchar>(x,y) = sqrt(sobelX.at<uchar>(x,y) * sobelX.at<uchar>(x,y) + sobelY.at<uchar>(x,y) * sobelY.at<uchar>(x,y));
                }    
            }

            //Calculating the interest points
            double Gx, Gy, GxGy; 
            //vector<Pixel> pixels;
            map <double, Pixel> pixels;
            int n = 1000000;
            double k[frame.rows][frame.cols];
            double min = n, max = -n;

            for(int y = 1; y < frame.rows-1 ; y++) {
                for(int x = 1; x < frame.cols-1 ; x++){
                    Gx =    sobelX.at<uchar>(y-1, x-1) + 
                            sobelX.at<uchar>(y, x-1) + 
                            sobelX.at<uchar>(y+1, x-1) + 
                            sobelX.at<uchar>(y-1, x) + 
                            sobelX.at<uchar>(y+1, x) +
                            sobelX.at<uchar>(y-1, x+1) +  
                            sobelX.at<uchar>(y, x+1) + 
                            sobelX.at<uchar>(y+1, x+1);
                    
                    Gy =    sobelY.at<uchar>(y-1, x-1) + 
                            sobelY.at<uchar>(y, x-1) + 
                            sobelY.at<uchar>(y+1, x-1) + 
                            sobelY.at<uchar>(y-1, x) + 
                            sobelY.at<uchar>(y+1, x) + 
                            sobelY.at<uchar>(y-1, x+1) + 
                            sobelY.at<uchar>(y, x+1) + 
                            sobelY.at<uchar>(y+1, x+1);

                    GxGy =  sobelX.at<uchar>(y-1,x-1) * sobelY.at<uchar>(y-1,x-1) +
                            sobelX.at<uchar>(y,x-1) * sobelY.at<uchar>(y,x-1) + 
                            sobelX.at<uchar>(y+1,x-1) * sobelY.at<uchar>(y+1,x-1) + 
                            sobelX.at<uchar>(y-1,x) * sobelY.at<uchar>(y-1,x) + 
                            sobelX.at<uchar>(y+1,x) * sobelY.at<uchar>(y+1,x) + 
                            sobelX.at<uchar>(y-1,x+1) * sobelY.at<uchar>(y-1,x+1) + 
                            sobelX.at<uchar>(y,x+1) * sobelY.at<uchar>(y,x+1) + 
                            sobelX.at<uchar>(y+1,x+1) * sobelY.at<uchar>(y+1,x+1);

                    k[y][x] =  ((((sobelX.at<uchar>(y,x) * sobelX.at<uchar>(y,x)) * (Gy*Gy)) 
                                        + ((sobelY.at<uchar>(y,x) * sobelY.at<uchar>(y,x)) * (Gx*Gx)) 
                                        - 2 * (sobelX.at<uchar>(y,x) * sobelY.at<uchar>(y,x)* GxGy )) 
                                        / ((Gy*Gy) + (Gx*Gx)));

                    if (k[y][x] * n < min * n) min = k[y][x];
                    if (k[y][x] * n > max * n) max = k[y][x];
                }
            }
        
             for(int x = 0; x < frame.rows ; x+=4) {
                for(int y = 0; y < frame.cols ; y+=4){
                    double max = -n;
                    Pixel p;
                    //Inserting in the vector
                    for(int i = x; i < x+4 ; i++) {
                        for(int j = y; j < y+4 ; j++){
                            k[x][y] += abs(min);
                            double v = k[x][y] * 255 / (max + abs(min));
                            uchar m = (uchar) v;
                        
                            if (v > max) {
                                p.color = m; p.x = y; p.y = x;
                                max = m;
                            }
                        }
                    }

                    pixels[p.color] = p;
                }
            }

            //Printing the interest points
            int number = 0;
            long maxAcc = -n*n;
            int acc[HEIGHT+256][WIDTH+256]; for (int i = 0; i < frame.rows+256; i++) for (int j = 0; j < frame.cols+256; j++) acc[i][j] = 0; //Initializing the Acc
            for (map<double, Pixel>::iterator i = pixels.end(); i != pixels.begin(); i--) {
                if (number == NB_INTEREST_POINTS) break;
                circle(frame, Point(i->second.x, i->second.y), 3, Scalar(0, 0, 255), 1);
                number++;

                double min = n*n;
                int Vx, Vy;
                maxAcc = -n;
                //Calculating the deplacement
                for (map<double, Pixel>::iterator j = pixels_old.end(); j != pixels_old.begin(); j--) {
                    if (number == NB_INTEREST_POINTS) break;
                    if (!(i->second.x == j->second.x && i->second.y == j->second.y)) {
                        double distance = sqrt(pow(i->second.x - j->second.x, 2) + pow(i->second.y - j->second.y, 2));
                        if (min < distance) {
                            min = distance;
//                            cout << distance << endl;
                            Vx = i->second.x - j->second.x;
                            Vy = i->second.y - j->second.y;
                            acc[Vx+255][Vy+255]++;
                            if (acc[Vx+255][Vy+255] > maxAcc) maxAcc = acc[Vx+255][Vy+255];
//                            cout << "Acc : " << acc[Vx+255][Vy+255] << endl;
  //                          cout << "x1 : " << i->second.x << ", x2 : " << j->second.x << endl;
  //                          cout << "y1 : " << i->second.y << ", y2 : " << j->second.y << endl;
//                            cout << ", Vx : " << Vx << ", Vy : " << Vy << endl;
                        }
                    }
                }
            }

            //cout << "Max acc : " << maxAcc << endl;
            //Calculation of the speed
            int speed = 0;
            int Vx, Vy;
            for (int i = 0; i < HEIGHT+256; i++) {
                for (int j = 0; j < WIDTH+256; j++) {
                    if (acc[i][j] > speed) {speed = acc[i][j]; Vx = i-255; Vy = j-255;}
                }
            }

            cout << "Speed : " << speed << ", Vx : " << Vy << ", Vy : " << Vy << endl;

            //Calculating the deplacement
            

            //Storing the old K matrix
            if (nbFrame == 3) {pixels_old = pixels; nbFrame = 0;}
            nbFrame++;
            
            imshow("MyCam", frame);
            imshow("Norme", norme);            
	    
        }
        if(waitKey(25) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
