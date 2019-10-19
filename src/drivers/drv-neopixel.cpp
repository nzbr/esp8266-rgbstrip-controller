//
// Created by nzbr on 13.08.2019.
//
#include "include.h"

#ifdef DRV_NEOPIXEL

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel* neoPixel;
int ledcount;

void drvInit(int leds, int pin) {
    neoPixel = new Adafruit_NeoPixel(leds, pin, NEOPIXEL_MODE);
    /*
     * Ein Mahnmal damit das NIE WIEDER passiert
     *
     * auto* newpixels = new Adafruit_NeoPixel(leds, pin, NEO_BRG + NEO_KHZ800);
     * neoPixel = *newpixels;
     * delete newpixels;
     */
    ledcount = leds;

    neoPixel->begin();
    neoPixel->setBrightness(128);
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
    neoPixel->setPixelColor(led, Adafruit_NeoPixel::Color(r, g, b));
}

void drvShow(){
    neoPixel->show();
}

#endif
