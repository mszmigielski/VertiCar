#pragma once
#include <Arduino.h>
#include "Config.h"

class ControlPanel {
    private:
        uint8_t _whiteButtonPin;
        uint8_t _whiteButtonLedPin;
        bool _whiteButtonState;
        bool readWhiteButton();
    public:
        ControlPanel(uint8_t whiteButtonPin = WHITE_BUTTON_PIN, uint8_t whiteButtonLedPin = WHITE_BUTTON_LED_PIN) : _whiteButtonPin(whiteButtonPin), _whiteButtonLedPin(whiteButtonLedPin), _whiteButtonState(false) {}
        void init();
        void update();
        bool isWhiteButtonPressed();
};