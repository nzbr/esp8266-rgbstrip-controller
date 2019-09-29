//
// Created by nzbr on 30.08.2019.
//

#include "include.h"

uint16_t timestamp = 0;
uint16_t lastUdpSignal = 0;
uint8_t* colorBuffer = nullptr;
int colorBufferLength = 0;
uint8_t* singleColor = nullptr;
int currentMode = -1;