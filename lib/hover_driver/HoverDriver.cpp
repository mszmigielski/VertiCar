#include <Arduino.h>
#include "HoverDriver.h"
#include "config.h"



 HoverDriver::HoverDriver(Stream& port) : _port(port) {
    // Konstruktor inicjalizujący referencję do portu szeregowego
    Command.start = (uint16_t)START_FRAME;
    Command.steer = 0;
    Command.speed = 0;
}

void HoverDriver::sendCommand() {
    Command.start    = (uint16_t)START_FRAME;
    Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Wysłanie paczki bajtów do płyty przez sprzętowy port
  _port.write((uint8_t *) &Command, sizeof(Command)); 
}

void HoverDriver::receiveFeedback() {
    static uint8_t idx = 0;
    static byte incomingBytePrev = 0;
    static SerialFeedback UnVerifiedFeedback; 

    while (_port.available()) { 
        byte incomingByte = _port.read();
        uint16_t bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;

        if (bufStartFrame == START_FRAME) {                     
            byte* byteBuffer = (byte*)&UnVerifiedFeedback;
            byteBuffer[0] = incomingBytePrev;
            byteBuffer[1] = incomingByte;
            idx = 2;
        } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  
            byte* byteBuffer = (byte*)&UnVerifiedFeedback;
            byteBuffer[idx] = incomingByte; 
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
               
            }
        }
        
        incomingBytePrev = incomingByte; 
    }
}


void HoverDriver::move(SpeedCommand& cmd){
    Command.steer = (int16_t)(cmd.steer * constraintSpeed);
    Command.speed = (int16_t)(cmd.speed * constraintSpeed);
    sendCommand();
    receiveFeedback();
}