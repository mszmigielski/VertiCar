#include "ControlPanel.h"

void ControlPanel::init() {
    pinMode(_whiteButtonPin, INPUT_PULLUP);
    pinMode(_whiteButtonLedPin, OUTPUT);
    digitalWrite(_whiteButtonLedPin, LOW); // Ensure LED is off initially
}

void ControlPanel::update() {
    _whiteButtonState = readWhiteButton();
    digitalWrite(_whiteButtonLedPin, _whiteButtonState ? HIGH : LOW); // Turn on LED if button is pressed
}

bool ControlPanel::readWhiteButton() {
    return digitalRead(_whiteButtonPin) == LOW; // Assuming active LOW button
}

bool ControlPanel::isWhiteButtonPressed() {
    return readWhiteButton();
}


