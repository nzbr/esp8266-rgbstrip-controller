//
// Created by nzbr on 13.08.2019.
//

#ifndef ESP8266_RGBSTRIP_CONTROLLER_CONFIG_H
#define ESP8266_RGBSTRIP_CONTROLLER_CONFIG_H

//Debug output
//#define DEBUG

//Choose a device driver
//#define DRV_NEOPIXEL
//#define DRV_TM1829
//#define DRV_PWM
//#define DRV_NEOPIXELBUS // to use neopixelbus you need to uncomment the right Feature for your leds https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object#neo-features

#ifdef DRV_NEOPIXEL
	#include <Adafruit_NeoPixel.h>
	const int NEOPIXEL_MODE = NEO_BRG + NEO_KHZ800;
#endif

#ifdef DRV_NEOPIXELBUS
	#define NEOPIXELBUS_FEATURE NeoGrbwFeature //for sk6812rgbw
#endif

//Device properties
#define DEVICE_PIN D6 // ignored on PWM
//#define R_PIN D6 // needed just for PWM
//#define G_PIN D7 // needed just for PWM
//#define B_PIN D8 // needed just for PWM
#define LED_COUNT 50

//Connect to WiFi and open UDP Port
#define WIFIENABLE
#define WIFISSID ""
#define WIFIPASS ""
#define TCPPORT 1337
#define UDPPORT 1337

//Reboot on UDP disconnect instead of switching to colorflow mode
#define UDP_REBOOT

//Read data from serial port (Baud is needed for output anyway!)
#define BAUDRATE 74880

void readConfig();
void storeConfig();
#endif //ESP8266_RGBSTRIP_CONTROLLER_CONFIG_H
