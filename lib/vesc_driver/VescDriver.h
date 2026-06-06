#pragma once
#include <Arduino.h>
#include "GlobalTypes.h"
#include "config.h"
#include "VescUart.h"





class VescDriver {
private:
    Stream& _port;  // Referencja do portu szeregowego
    VescUart Vesc;
    float maxOnCurrentDuty = MAX_ON_CURRENT_DUTY; // Maksymalne obroty silnika (do dostosowania w zależności od silnika i ustawień VESC)
    struct VescDuty {
        float dutyL=0;
        float dutyR=0;
    };
    VescDuty CommandDuty;

    struct VescCurrent {
        float currentL=0;
        float currentR=0;
    };
    VescCurrent CommandCurrent;
    //#TODO: usunąć powyższe struktury i zastąpić bezpośrednio w metodach setDuty i setCurrent

public:

    VescDriver(Stream& port);
    void setDuty(SpeedCommand& cmd);
    void setCurrent(SpeedCommand& cmd);
    void setBrakeCurrent(float brakeCurrent);
    bool update();
    
    VescUart::dataPackage dataL;
    VescUart::dataPackage dataR;
//TODO: add method to get data from object
};

