//
// Created by nzbr on 28.08.2019.
//

#include "colors.h"

#ifndef ESP8266_RGBSTRIP_CONTROLLER_SETTINGS_H
#define ESP8266_RGBSTRIP_CONTROLLER_SETTINGS_H

uint8_t getDefaultMode();
void setDefaultMode(uint8_t m);
color getDefaultSingleColor();
void setDefaultSingeColor(color c);

#endif //ESP8266_RGBSTRIP_CONTROLLER_SETTINGS_H
