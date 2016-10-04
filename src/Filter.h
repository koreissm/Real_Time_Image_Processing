#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

#define HEIGHT 240
#define WIDTH 320

using namespace cv;

void getBlueImage(Mat *frame, int blueTresh, double tresh);

void getGravityCenter(Mat *frame, int *xMin, int *xMax, int *yMin, int *yMax);
