#pragma once
#include <Arduino.h>
#include "GlobalTypes.h"
#include "config.h"
class HoverDriver {
private:
    Stream& _port;  // Referencja do portu szeregowego
    SerialCommand Command;
    SerialFeedback Feedback;
    int16_t constraintSpeed = MAX_SPEED;
    void sendCommand();
    void receiveFeedback();
public:
    HoverDriver(Stream& port);
    void move(SpeedCommand& cmd);
};
