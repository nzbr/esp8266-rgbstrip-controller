//
// Created by nzbr on 13.08.2019.
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "include.h"

#include "animations/bootanimation.h"
#include "animations/connecting.h"
#include "modes/mode.h"

void receivePackets();
String processPacket(int len, const uint8_t* data);

WiFiUDP *UDP;
WiFiServer *TCP;
bool connected = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
#ifndef DRV_NEOPIXELBUS
    Serial.begin(BAUDRATE);
#endif
    drvInit(LED_COUNT, DEVICE_PIN);
    bootAnimation();

    #ifdef WIFIENABLE
        WiFi.mode(WIFI_STA); //Disable AccessPoint
        WiFi.begin(WIFISSID, WIFIPASS);
#ifndef DRV_NEOPIXELBUS
        Serial.println("\nConnecting");
#endif
        TCP = new WiFiServer(TCPPORT);
        TCP->begin();

        UDP = new WiFiUDP();
        UDP->begin(UDPPORT);
    #endif

    digitalWrite(LED_BUILTIN, HIGH);
}

void loop(){
    timestamp++;
    #ifdef DEBUG
#ifndef DRV_NEOPIXELBUS
        if (timestamp % 256 == 0) Serial.println(timestamp);
#endif
    #endif

    #ifdef WIFIENABLE
        //TODO: Add configurable timeout
        if (WiFi.status() != WL_CONNECTED) {
            if (connected) {
#ifndef DRV_NEOPIXELBUS
                Serial.println("\nReconnecting");
#endif
                connected = false;
            }
            connectingStep();
            return;
        }
        if (!connected) {
            connectingFree();
#ifndef DRV_NEOPIXELBUS
            Serial.println("Connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            Serial.println();
#endif
            connected = true;
        }
    #endif

    receivePackets();
    yield(); // Reset watchdog, to give modeStep() the full time
    modeStep();
}

void receivePackets() {
#ifndef DRV_NEOPIXELBUS
    if (Serial.available()) {
        uint8_t len = Serial.read();
        if (Serial.available() < len) {
            //Dispose invalid data
            while (Serial.read() != -1) {}
            goto skipSerial;
        }
        uint8_t buffer[len];
        int read = Serial.readBytes(buffer, len);
        if (read < len) {
            Serial.println("Serial read error!");
            goto skipSerial;
        }
        Serial.println(processPacket(read, buffer));
        free(buffer);
    }
    skipSerial:
#endif

    #ifdef WIFIENABLE
        //TCP => Serial without length bit
        WiFiClient client = TCP->available();
        if (client) {
            int len = client.available();
            if (len) {
                auto* buffer = (uint8_t*)malloc(sizeof(uint8_t)*len);
                int read = client.read(buffer, len);
                if (read < len) {
                    client.print("TCP read error");
                    goto skipTCP;
                }
                client.print(processPacket(read, buffer));
                free(buffer);
            }
            skipTCP:
            client.stop();
        }

        //UDP => Raw color data for aurora mode, no response
        //Skip to last packet
        #ifdef DEBUG
            int missed = -1;
        #endif

        int len;
        while (UDP->parsePacket()) {
            lastUdpSignal = timestamp;

            #ifdef DEBUG
                missed++;
            #endif
            len = UDP->available();
            if (currentMode != m_aurora) setMode(m_aurora); //Auto-Switch to aurora mode
            if (len % 3 == 0){
                if (colorBufferLength != len) {
                    colorBufferLength = len;
                    free(colorBuffer);
                    colorBuffer = (uint8_t*)malloc(sizeof(uint8_t)*colorBufferLength);
                }
                UDP->read(colorBuffer, colorBufferLength);
            }
        }
        #ifdef DEBUG
            if (missed > 0) Serial.printf("[%5d] Missed %d UDP packets\n", timestamp, missed);
        #endif

    #endif
}

//This is only for TCP and Serial, UDP is only used for raw color data
//First returned letter is E for Error or S for Success
String processPacket(int len, const uint8_t* data) {
    switch (data[0]) {
        case 0: //Raw color data
            if ((len-1) % 3 != 0) {
                return "E Invalid color data received (payload length not divisible by 3)";
            }
            if (colorBuffer != nullptr) {
                if (colorBufferLength != len - 1) {
                    colorBufferLength = len - 1;
                    free(colorBuffer);
                    colorBuffer = (uint8_t*)malloc(sizeof(uint8_t)*colorBufferLength);
                }
                memcpy(colorBuffer, &data[1], len-1);
                return "S Received data";
            }
            return "S Ignored";
        case 1: //Set Mode
            if (len != 2) {
                return "E Invalid payload length for mode selection";
            }
            setMode(data[1]);
            return "S Mode Set";
        case 2: //Set solid color
            if (len != 4) {
                return "E Invalid payload length for setting color";
            }
            if (singleColor != nullptr) {
                memcpy(singleColor, &data[1], 3);
                return "S Color Set";
            }
            return "S Ignored";
        case 3: //Read settings
        case 4: //Write settings
            return "E Not Implemented!";
        case 0xFE: //Heartbeat
            return "S";
        case 0xFF: //Reset
            Serial.println("Reset!");
            delay(10);
            ESP.reset();
            return "E Unknown Error";
        default:
            return "E Unknown Command";
    }
}
