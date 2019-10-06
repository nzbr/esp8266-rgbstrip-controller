//
// Created by Philipp Hochkamp on 03.10.2019.
//
#include "include.h"

#ifdef DRV_NEOPIXELBUS

#include <NeoPixelBus.h>

NeoPixelBus<NEOPIXELBUS_FEATURE, Neo800KbpsMethod> npb(LED_COUNT, DEVICE_PIN);
int ledcount;

void drvInit(int leds, int pin) {
    ledcount = leds;
    npb.Begin();
}

void drvSetStrip(uint8_t r, uint8_t g, uint8_t b){
    for (int i = 0; i < ledcount; i++) {
        drvSetPixel(i, r, g, b);
    }
}

void drvSetPixel(unsigned int led, uint8_t r, uint8_t g, uint8_t b){
    /*if (r == 255) r--; //TODO: Only helped partially
    if (g == 255) g--;
    if (b == 255) b--;*/
    npb.SetPixelColor(led, RgbwColor(r,g,b));
}

void drvShow(){
    npb.Show();
}

#endif
