#pragma once
#include <Arduino.h>
#include "HoverDriver.h"
#include "config.h"



 HoverDriver::HoverDriver(Stream& port) {
    // Konstruktor inicjalizujący referencję do portu szeregowego
    _port = port;
    Command.start = (uint16_t)START_FRAME;
    Command.steer = 0;
    Command.speed = 0;
}

void HoverDriver::sendCommand(HoverboardCommand& cmd) {
    Command.start    = (uint16_t)START_FRAME;
    Command.steer    = cmd.steer;
    Command.speed    = cmd.speed;
    Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Wysłanie paczki bajtów do płyty przez sprzętowy port
  _port.write((uint8_t *) &Command, sizeof(Command)); 
}

void HoverDriver::receiveFeedback() {
    static uint8_t idx = 0;
    uint16_t bufStartFrame;
    byte incomingByte;
    static byte incomingBytePrev;
    SerialFeedback UnVerifiedFeedback;

    if(_port.available()) {
        incomingByte = _port.read();
        bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;
    } else {
        return;
    }
    if (bufStartFrame == START_FRAME) {                     
       byte* byteBuffer = (byte*)&UnVerifiedFeedback;
        byteBuffer[0] = incomingBytePrev;
        byteBuffer[1] = incomingByte;
        idx = 2;
    } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  
        byte* byteBuffer = (byte*)&UnVerifiedFeedback;
        byteBuffer[idx] = incomingByte; // Bezpieczne wpisywanie indeksowe
        idx++;
    }
    if (idx == sizeof(SerialFeedback)) {
        uint16_t checksum;
        idx = 0;
        checksum = (uint16_t)(UnVerifiedFeedback.start ^ UnVerifiedFeedback.cmd1 ^ UnVerifiedFeedback.cmd2 ^ 
                              UnVerifiedFeedback.speedR_meas ^ UnVerifiedFeedback.speedL_meas ^ 
                              UnVerifiedFeedback.batVoltage ^ UnVerifiedFeedback.boardTemp ^ UnVerifiedFeedback.cmdLed);

        if (UnVerifiedFeedback.checksum == checksum) {
            Feedback = UnVerifiedFeedback;
        } else {
            // To Do: Obsługa błędu sumy kontrolnej (np. logowanie, ponowne żądanie danych itp.)
        }
    incomingBytePrev = incomingByte;
    }
}


void HoverDriver::move(HoverboardCommand& cmd){
    sendCommand(cmd);
    receiveFeedback();
}