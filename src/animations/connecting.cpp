//
// Created by nzbr on 29.08.2019.
//

#include "include.h"
#include "connecting.h"

bool initialized = false;
unsigned int pos = 0;
int absstep = 0;
int step = 1;
int cycle = 0;

#define MAXPOS 128


void connectingStep() {
    if (!initialized) {
        if (absstep == 0) {
            absstep = max((int)round(LED_COUNT / 100), 1);
            Serial.println(absstep);
        }
        drvSetStrip(C_BLACK);
        initialized = true;
        pos = 0;

        #ifdef WIFI_REBOOT
        cycle = 0;
        #endif
    };

    #ifdef WIFI_REBOOT
    cycle++;
    if (cycle == WIFI_TIMEOUT) {
        ESP.reset();
    }
    #endif
    
    if (pos >= MAXPOS) {
        step = -absstep;
    } else if (pos <= 0) {
        step = absstep;
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