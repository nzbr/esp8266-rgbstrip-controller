//
// Created by nzbr on 15.08.2019.
//

#ifndef ESP8266_RGBSTRIP_CONTROLLER_COLORS_H
#define ESP8266_RGBSTRIP_CONTROLLER_COLORS_H

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

#define C_WHITE 255, 255, 255
#define C_BLACK 0, 0, 0
#define C_RED 255, 0, 0
#define C_GREEN 0, 255, 0
#define C_BLUE 0, 0, 255

#endif //ESP8266_RGBSTRIP_CONTROLLER_COLORS_H
