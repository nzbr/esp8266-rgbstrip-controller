//
// Created by nzbr on 29.08.2019.
//

#ifndef ESP8266_RGBSTRIP_CONTROLLER_MODE_H
#define ESP8266_RGBSTRIP_CONTROLLER_MODE_H

#define m_solidColor 0
#define m_colorFlow 1
#define m_network 2
#define m_default 0xFF

void modeStep();
void setMode(uint8_t mode);

#endif //ESP8266_RGBSTRIP_CONTROLLER_MODE_H
