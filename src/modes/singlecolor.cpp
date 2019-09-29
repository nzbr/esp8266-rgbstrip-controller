//
// Created by nzbr on 30.08.2019.
//

#include "../include.h"
#include "singlecolor.h"

void setToDefaultColor();

void singleColorStep() {
    if (singleColor == nullptr) {
        setToDefaultColor();
    }
    drvSetStrip(singleColor[0], singleColor[1], singleColor[2]);
    drvShow();
}

void singleColorFree() {
    free(singleColor);
    singleColor = nullptr;
}

void setToDefaultColor() {
    singleColor = (uint8_t*)calloc(3, sizeof(uint8_t));
    singleColor[0] = 0x00;
    singleColor[1] = 0xFF;
    singleColor[2] = 0x00;
}