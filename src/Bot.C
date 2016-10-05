#include "Bot.h"


void giveSomeOrders(FILE* arduino, int x1, int y1, int x2, int y2) {
    int xMove = 0, yMove = 0;
    
    if (x1 < x2) {//Go Right
        xMove = X_MOVE;
    }
    else if (x1 > x2) {//Go left
        xMove = -X_MOVE;
    }

    if (y1 < y2) {//Go Down
        yMove = Y_MOVE;
    }
    else if (y1 > y2) {//Go up
        yMove = -Y_MOVE;
    }
    
    fprintf(arduino,"%d:%d,", xMove, yMove); //Writing to the file
    
}
