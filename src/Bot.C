#include "Bot.h"


void giveSomeOrders(int x1, int y1, int x2, int y2) {
    FILE *file;
    file = fopen(DEVICE_NAME, "w");  //Opening device file
    //fprintf(file,"%s",argv[2]); //Writing to the file
    //fprintf(file,"%c",','); //To separate digits
    
    if (x1 < x2) {//Go Right

    }
    else {//Go left
        
    }

    if (y1 < y2) {//Go Down

    }
    else {//Go up

    }
    
    fclose(file);
}
