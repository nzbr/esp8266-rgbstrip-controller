//
// Created by nzbr on 29.08.2019.
//

#include "include.h"
#include "connecting.h"

bool initialized = false;
unsigned int pos = 0;
int step = 1;
#define IND_LEN 5


void connectingStep() {
    if (!initialized) drvSetStrip(C_BLACK);
    for (unsigned int p = pos; p < pos+IND_LEN; p++) {
        drvSetPixel(p, C_BLUE);
    }
    if (step == 1) {
        if (pos != 0) {
            drvSetPixel(pos - 1, C_BLACK);
        }
    } else {
        if (pos + IND_LEN != LED_COUNT) {
            drvSetPixel(pos + IND_LEN + 1, C_BLACK);
        }
    }
    pos += step;
    if (pos == 0 || pos + IND_LEN == LED_COUNT) {
        step *= -1;
    }
    drvShow();
    delay(25);
}

void connectingFree() {
    drvSetStrip(C_BLACK);
    drvShow();
}