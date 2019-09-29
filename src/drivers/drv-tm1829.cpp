//
// Created by nzbr on 13.08.2019.
//
//Muss davor stehen, damit die variable ggf gesetzt ist
#include "include.h"

#ifdef DRV_TM1829

#include "user_interface.h"

static inline uint32_t _getCycleCount(void);
void tm1829_write_to_pin(uint8_t pin, uint8_t *pixels, uint32_t length);

uint8_t ledArr[LED_COUNT * 3];

void drvInit(int leds, int pin) {
    pinMode(DEVICE_PIN, OUTPUT);
    digitalWrite(DEVICE_PIN, HIGH);
    delay(1);
    drvSetStrip(C_BLACK);
}

void drvSetStrip(uint8_t r, uint8_t g, uint8_t b){
    for (int i = 0; i < LED_COUNT; i++) {
        drvSetPixel(i, r, g, b);
    }
}

void drvSetPixel(unsigned int led, uint8_t r, uint8_t g, uint8_t b) {
    if (led < LED_COUNT) {
        //Prevent LED strip crashes
        if (r == 255) r = 254;
        if (g == 255) g = 254;
        if (b == 255) b = 254;
        ledArr[led*3]   = b;
        ledArr[led*3+1] = r;
        ledArr[led*3+2] = g;
    }
}

void drvShow(){
    tm1829_write_to_pin(DEVICE_PIN, ledArr, LED_COUNT*3);
}

//Code from https://nodemcu.readthedocs.io/en/latest/en/modules/tm1829/
void tm1829_write_to_pin(uint8_t pin, uint8_t *pixels, uint32_t length) {
    uint8_t *p, *end;

    p   =  pixels;
    end =  p + length;

    const uint32_t t0l  = (1000 * system_get_cpu_freq()) / 3333;  // 0.390us (spec=0.35 +- 0.15)
    const uint32_t t1l  = (1000 * system_get_cpu_freq()) / 1250;  // 0.800us (spec=0.70 +- 0.15)

    const uint32_t ttot = (1000 * system_get_cpu_freq()) / 800;   // 1.25us

    while (p != end) {

        register int i;

        register uint8_t pixel = *p++;

        ets_intr_lock();
        for (i = 7; i >= 0; i--) {
            register uint32_t pin_mask = 1 << pin;

            // Select low time
            register uint32_t tl = ((pixel >> i) & 1) ? t1l : t0l;
            register uint32_t t1, t2;

            register uint32_t t = _getCycleCount();

            t1 = t + tl;
            t2 = t + ttot;
            //digitalWrite(outputPin, LOW);
            GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pin_mask); // Set pin low
            while (_getCycleCount() < t1);

            //digitalWrite(outputPin, HIGH);
            GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pin_mask); // Set pin high
            while (_getCycleCount() < t2);
        }

        ets_intr_unlock();
    }
}
static inline uint32_t _getCycleCount(void) {
    uint32_t cycles;
    __asm__ __volatile__("rsr %0,ccount":"=a" (cycles));
    return cycles;
}


#endif
