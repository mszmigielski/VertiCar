#pragma once
#include <Arduino.h>
#include "GlobalTypes.h"
#include "config.h"
#include "VescUart.h"





class VescDriver {
private:
    Stream& _port;  // Referencja do portu szeregowego
    VescUart Vesc;
    int16_t constraintSpeed = MAX_SPEED;

    struct VescCommand {
        float dutyL;
        float dutyR;
    };
    VescCommand Command;
    
public:

    VescDriver(Stream& port);
    void move(SpeedCommand& cmd);
    bool update();
    
    VescUart::dataPackage dataL;
    VescUart::dataPackage dataR;

};

