#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <vector>

#define NB_INTEREST_POINTS 20

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
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH,320);  //taille de la fenetre
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240); //au dela de 320*240, image qui lag


    MatIterator_<Vec3b> it, end;

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
            Mat k;
            double Gx, Gy, GxGy; 
            k = frame.clone();
            vector<Pixel> pixels;
            int n = 1000000;
            double min = n, max = -n;

            for(int y = 1; y < k.rows-1 ; y++) {
                for(int x = 1; x < k.cols-1 ; x++){
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

                    k.at<long>(y, x) =  ((((sobelX.at<uchar>(y,x) * sobelX.at<uchar>(y,x)) * (Gy*Gy)) 
                                        + ((sobelY.at<uchar>(y,x) * sobelY.at<uchar>(y,x)) * (Gx*Gx)) 
                                        - 2 * (sobelX.at<uchar>(y,x) * sobelY.at<uchar>(y,x)* GxGy )) 
                                        / ((Gy*Gy) + (Gx*Gx)));

                    if (k.at<long>(y, x) * n < min * n) min = k.at<long>(y, x);
                    if (k.at<long>(y, x) * n > max * n) max = k.at<long>(y, x);
//                    cout << "k : " << k.at<long>(y, x) << endl;
                }
            }
        
//            cout << "Min : " << min << ", Max : " << max << endl;

            
             for(int x = 0; x < k.rows ; x++) {
                for(int y = 0; y < k.cols ; y++){
                    k.at<long>(x, y) += abs(min);
                    double v = k.at<long>(x, y) * 255 / (max + abs(min));
                    uchar m = (uchar) v;
                    Pixel p; p.color = m; p.x = x; p.y = y;
                    //if (m > 150) circle(frame, Point(x, y), 8, Scalar(0, 0, 255), 1); 
                    //Inserting in the vector
                    if (pixels.size() < NB_INTEREST_POINTS)
                        pixels.push_back(p);
                    else {
                        pixels.erase(pixels.begin());
                        pixels.push_back(p);
                        sort(pixels.begin(), pixels.end(), comparePixels);
                    }
                }
            }

            //Printing the interest points
            for (vector<Pixel>::iterator it=pixels.begin(); it!=pixels.end(); ++it) {
                circle(frame, Point((*it).x, (*it).y), 1, Scalar(0, 0, 255), 2);
            }

            imshow("MyCam", frame);
            imshow("Norme", norme);            
	    
        }
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
