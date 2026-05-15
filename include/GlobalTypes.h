#pragma once
#include <Arduino.h>
typedef struct{
    int16_t speed;
    int16_t steer;
} HoverboardCommand;

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
