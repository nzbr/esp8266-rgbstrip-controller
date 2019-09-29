//
// Created by nzbr on 30.08.2019.
//

#ifndef ESP8266_RGBSTRIP_CONTROLLER_SHMEM_H
#define ESP8266_RGBSTRIP_CONTROLLER_SHMEM_H

extern uint16_t timestamp;
extern uint16_t lastUdpSignal;
extern uint8_t* colorBuffer;
extern int colorBufferLength;
extern uint8_t* singleColor;
extern int currentMode;

#endif //ESP8266_RGBSTRIP_CONTROLLER_SHMEM_H
