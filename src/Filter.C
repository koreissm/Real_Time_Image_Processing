#include"Filter.h"

using namespace std;

void getBlueImage(Mat *frame, int blueTresh, double tresh) {
    MatIterator_<Vec3b> it, end;
    char blueImage[HEIGHT][WIDTH];
    int i, j;
    
    for( it = frame->begin<Vec3b>(), end = frame->end<Vec3b>(); it != end; ++it) {
        int r = (*it)[2]; /*accès au R */
        int g = (*it)[1];  /* le G */
        int b = (*it)[0]; /* le B*/
                
        if (!(b > blueTresh && r < b*tresh && g < b*tresh)) { // If blue pixel
            (*it)[2] = 0;
            (*it)[1] = 0;
            (*it)[0] = 0;
        }
    }

}

void getGravityCenter(Mat *frame, int *x, int *y) {
    int i, j;
    int xMin = 0, xMax = 0, yMin = 0, yMax = 0;
    MatIterator_<Vec3b> it, end;
    
    for( it = frame->begin<Vec3b>(), end = frame->end<Vec3b>(); it != end; ++it) {
        int r = (*it)[2]; /*accès au R */
	    int g = (*it)[1];  /* le G */
        int b = (*it)[0]; /* le B*/

        if (j == WIDTH) {i++; j = 0;}
            if (r != 0 && g != 0 && b != 0) {
            if (xMin == 0) {xMin = j; (*it)[2] = (*it)[1] = (*it)[0] = 255;}
            if (yMin == 0) {yMin = i; (*it)[2] = (*it)[1] = (*it)[0] = 255;} //Most top-left pixel
            if (j > xMax) {xMax = j; (*it)[2] = (*it)[1] = (*it)[0] = 255;}
            if (i > yMax) {yMax = i; (*it)[2] = (*it)[1] = (*it)[0] = 255;} 
         }

        j++;
    }
    *x = (xMin + xMax)/2;
    *y = (yMin + yMax)/2;
} 
