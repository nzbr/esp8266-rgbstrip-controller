//
// Created by nzbr on 13.08.2019.
//

#ifndef ESP8266_RGBSTRIP_CONTROLLER_DRV_H
#define ESP8266_RGBSTRIP_CONTROLLER_DRV_H

void drvInit(int leds, int pin);
void drvSetStrip(uint8_t r, uint8_t g, uint8_t b);
void drvSetPixel(unsigned int led, uint8_t r, uint8_t g, uint8_t b);
void drvShow();

#endif //ESP8266_RGBSTRIP_CONTROLLER_DRV_H
