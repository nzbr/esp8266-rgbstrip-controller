//
// Created by nzbr on 29.08.2019.
//

#include "include.h"
#include "connecting.h"

bool initialized = false;
unsigned int pos = 0;
int step = 1;

#define STEP 3


void connectingStep() {
    if (!initialized) {
        drvSetStrip(C_BLACK);
        initialized = true;
        pos = 0;
    };
    
    if (pos >= 255) {
        step = -STEP;
    } else if (pos <= 0) {
        step = STEP;
    }
    pos += step;

    drvSetStrip(0, pos, pos);
    drvShow();
    #ifdef DRV_TM1829
        delay(1);
    #endif
}

void connectingFree() {
    initialized = false;
    drvSetStrip(C_BLACK);
    drvShow();
}