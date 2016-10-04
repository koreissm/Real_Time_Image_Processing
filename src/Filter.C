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
                
        if (!(b > blueTresh && r < b*tresh && g < b*tresh && r < 100 && g < 100)) { // If blue pixel
            (*it)[2] = 0;
            (*it)[1] = 0;
            (*it)[0] = 0;
        }
    }

}

void getGravityCenter(Mat *frame, int *xMin, int *xMax, int *yMin, int *yMax) {
    int i = 0, j = 0;
    *xMin = WIDTH; *xMax = 0; *yMin = HEIGHT; *yMax = 0;
    MatIterator_<Vec3b> it, end;
    
    for( it = frame->begin<Vec3b>(), end = frame->end<Vec3b>(); it != end; ++it) {
        int r = (*it)[2]; /*accès au R */
	    int g = (*it)[1];  /* le G */
        int b = (*it)[0]; /* le B*/
    
        //Detecting
        if (j == WIDTH) {i++; j = 0;}
        if (r != 0 && g != 0 && b != 0) {
            if (j < *xMin) {*xMin = j;}
            if (i < *yMin) {*yMin = i;} //Most top-left pixel
            if (j > *xMax) {*xMax = j;}
            if (i > *yMax) {*yMax = i;} 
         }

        j++;
    }
} 
