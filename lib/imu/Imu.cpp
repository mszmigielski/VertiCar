#include "Imu.h"


bool Imu::init(){
    return _mpu.setup(0x68, MPU9250Setting(), _wire);
            //TODO: custom settings and calibration
}

bool Imu::update() {
    if (_mpu.update()) {
        _yaw = _mpu.getYaw();
        _pitch = _mpu.getPitch();
        _roll = _mpu.getRoll();
        return true;
    }
    return false;
}

