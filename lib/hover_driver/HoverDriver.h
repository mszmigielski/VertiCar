#pragma once
#include <Arduino.h>
#include "GlobalTypes.h"
#include "config.h"
class HoverDriver {
private:
    Stream& _port;  // Referencja do portu szeregowego
    SerialCommand Command;
    SerialFeedback Feedback;
    void sendCommand(HoverboardCommand& cmd);
    void receiveFeedback();
public:
    HoverDriver(Stream& port) : _port(port) {}
    void move(HoverboardCommand& cmd);
};
