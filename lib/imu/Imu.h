#pragma once
#include <Arduino.h>
#include "MPU9250.h"


class Imu {
    private:
        MPU9250 _mpu;
        float _yaw, _pitch, _roll;
        TwoWire& _wire;
    public:
        Imu(TwoWire& wire = Wire) : _wire(wire) {}
        bool init() ;
        bool update();
            
        float getYaw() const { return _yaw; }
        float getPitch() const { return _pitch; }
        float getRoll() const { return _roll; }
};