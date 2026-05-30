#pragma once
#include <Arduino.h>

// Val [-1, 1]
typedef struct{
    float speed;
    float steer;
} SpeedCommand;

typedef struct{
   float dutyL;
   float dutyR;
} VescCommand;

typedef struct {
   uint16_t start;
   int16_t  steer;
   int16_t  speed;
   uint16_t checksum;
} __attribute__((packed)) SerialCommand;

typedef struct {
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} __attribute__((packed)) SerialFeedback;
//To do: wywalić typedef