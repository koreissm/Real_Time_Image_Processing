#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define DEVICE_NAME "/dev/cu.usbmodem14211" 
#define X_MOVE 5
#define Y_MOVE 5

void giveSomeOrders(FILE* arduino, int x1, int y1, int x2, int y2);
