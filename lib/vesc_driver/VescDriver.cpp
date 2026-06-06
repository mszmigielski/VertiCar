#include "VescDriver.h"
#include "GlobalTypes.h"
#include "config.h"

VescDriver::VescDriver(Stream& port) : _port(port) {
    // Konstruktor inicjalizujący referencję do portu szeregowego
    Vesc.setSerialPort(&_port); 
    //Vesc.setDebugPort(&Serial);
}



void VescDriver::setDuty(SpeedCommand& cmd) {
    CommandDuty.dutyL = (cmd.speed + cmd.steer)*(float)MAX_DUTY; // Lewa strona: prędkość + skręt
    CommandDuty.dutyR = (cmd.speed - cmd.steer)*(float)MAX_DUTY; // Prawa strona: prędkość - skręt
    Vesc.setDuty(CommandDuty.dutyR);       
    Vesc.setDuty(CommandDuty.dutyL, 2);
}
void VescDriver::setCurrent(SpeedCommand& cmd) {
    CommandCurrent.currentL = (cmd.speed + cmd.steer)*(float)MAX_CURRENT; // Lewa strona: prędkość + skręt
    CommandCurrent.currentR = (cmd.speed - cmd.steer)*(float)MAX_CURRENT; // Prawa strona: prędkość - skręt
    if(abs(dataL.dutyCycleNow) > maxOnCurrentDuty){
        CommandCurrent.currentL = 0; 
    } else {
        Vesc.setCurrent(CommandCurrent.currentL, 2);
    }

    if(abs(dataR.dutyCycleNow) > maxOnCurrentDuty){
        CommandCurrent.currentR = 0; 
    } else {
        Vesc.setCurrent(CommandCurrent.currentR);
    }

    
}

void VescDriver::setBrakeCurrent(float brakeCurrent) {
    Vesc.setBrakeCurrent(brakeCurrent);
    Vesc.setBrakeCurrent(brakeCurrent, 2);
}



bool VescDriver::update() {
    // Tutaj można dodać kod do aktualizacji stanu kierowcy, np. odczyt danych z VESC
    bool state = Vesc.getVescValues();
    dataR = Vesc.data; // Zakładam, że Vesc.data jest aktualizowane przez getVescValues() 
    state = state && Vesc.getVescValues(2);
    dataL = Vesc.data; // Zakładam, że Vesc.data jest aktualizowane przez getVescValues() 
    
    return state;
}



