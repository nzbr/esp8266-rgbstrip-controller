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
//#define DRV_NEOPIXELBUS // to use neopixelbus you need to uncomment the right Feature for your leds https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object#neo-features
//#define NEOPIXELBUS_FEATURE NeoGrbwFeature //for sk6812rgbw

//Device properties
#define DEVICE_PIN D6
#define LED_COUNT 50

//Connect to WiFi and open UDP Port
#define WIFIENABLE
#define WIFISSID ""
#define WIFIPASS ""
#define TCPPORT 1337
#define UDPPORT 1337

//Read data from serial port (Baud is needed for output anyway!)
#define BAUDRATE 74880

void readConfig();
void storeConfig();
#endif //ESP8266_RGBSTRIP_CONTROLLER_CONFIG_H
