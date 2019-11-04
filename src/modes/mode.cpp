//
// Created by nzbr on 29.08.2019.
//

#include "../include.h"
#include "mode.h"

#include "colorflow.h"
#include "singlecolor.h"
#include "network.h"

void setToDefaultMode();

void (*stepFunction)();
void (*freeFunction)();

void modeStep(){
    if (stepFunction == nullptr){
        setToDefaultMode();
    } else {
        stepFunction();
    }
};

void setMode(uint8_t mode){
    if (currentMode == mode) return;

    if (freeFunction != nullptr) {
        freeFunction();
    }

    currentMode = mode;
    switch (mode) {
        case m_solidColor:
            stepFunction = &singleColorStep;
            freeFunction = &singleColorFree;
            break;
        case m_colorFlow:
            stepFunction = &colorFlowStep;
            freeFunction = &colorFlowFree;
            break;
        case m_network:
            stepFunction = &networkStep;
            freeFunction = &networkFree;
            break;
        case m_default:
        default:
            currentMode = -1;
            stepFunction = nullptr;
            freeFunction = nullptr;
    }
}

void setToDefaultMode() {
    //TODO: Read this from EEPROM
    setMode(m_colorFlow);
}