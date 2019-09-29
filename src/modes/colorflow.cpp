//
// Created by nzbr on 13.08.2019.
//
#include "colorflow.h"

void colorFlowSetup();
void generateColor(int x);

#define CYCLES 1024
#define DURATION 5000.0

static uint8_t** colors;
static int cycclock;
static int modstep;
static bool initialized = false;

void colorFlowSetup() {
    colors = (uint8_t**)calloc(CYCLES, sizeof(uint8_t*)); //Initialize with NULL pointers
    modstep = CYCLES / LED_COUNT;
    cycclock = -1;

    initialized = true;
}

void colorFlowStep() {
    bool generatedThisCycle = false;
    if (! initialized) colorFlowSetup();
    cycclock = (cycclock+1) % CYCLES;
    for (int i = 0; i < LED_COUNT; i++) {
        int modclock = cycclock - modstep*i;
        while (modclock < 0) modclock += CYCLES;

        uint8_t r = 0, g = 0, b = 0;
        if (colors[modclock] == NULL) {
            if (i == 0) {
                generatedThisCycle = true;
                generateColor(modclock);
            } else {
                continue;
            }
        }
        if (colors[modclock] != NULL) {
            r = colors[modclock][0];
            g = colors[modclock][1];
            b = colors[modclock][2];
        }
        drvSetPixel(i, r, g, b);
    }
    //DEBUG: Serial.printf("%4u/%4u, [%3u %3u %3u]\n", cycclock, CYCLES, colors[cycclock][0], colors[cycclock][1], colors[cycclock][2]);
    drvShow();
    if (generatedThisCycle) {
        yield();
    } else {
        delay(ceil(DURATION/CYCLES));
    }
}

void colorFlowFree() {
    initialized = false;
    for (int i = 0; i < CYCLES; i++) {
        free(colors[i]);
    }
    free(colors);
}

void generateColor(int x) {
    colors[x] = (uint8_t*)malloc(sizeof(uint8_t) * 3);

    //You know it's serious math because there are greek letters
    double p = (2.0 / CYCLES) * PI * x;
    colors[x][0] = floor((sin(p) + 1) * 127);
    colors[x][1] = floor((sin(p + 2.0 * PI / 3.0) + 1) * 127);
    colors[x][2] = floor((sin(p + 4.0 * PI / 3.0) + 1) * 127);
}