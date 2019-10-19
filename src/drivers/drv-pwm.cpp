//
// Created by Philipp Hochkamp on 03.10.2019.
//
#include "include.h"

#ifdef DRV_PWM

void drvInit(int leds, int pin) {
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
    digitalWrite(R_PIN, true);
    digitalWrite(G_PIN, true);
    digitalWrite(B_PIN, true);
}

void drvSetStrip(uint8_t r, uint8_t g, uint8_t b){
    analogWrite(R_PIN, r);
    analogWrite(G_PIN, g);
    analogWrite(B_PIN, b);
}

void drvSetPixel(unsigned int led, uint8_t r, uint8_t g, uint8_t b){
    analogWrite(R_PIN, r);
    analogWrite(G_PIN, g);
    analogWrite(B_PIN, b);
}

void drvShow(){
}

#endif
