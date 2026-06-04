#include "VescDriver.h"
#include "GlobalTypes.h"
#include "config.h"

VescDriver::VescDriver(Stream& port) : _port(port) {
    // Konstruktor inicjalizujący referencję do portu szeregowego
    Vesc.setSerialPort(&_port); // Przekaż referencję do portu szeregowego do obiektu VescUart
    Command.dutyL = 0;
    Command.dutyR = 0;
}

void VescDriver::move(SpeedCommand& cmd) {
    Command.dutyL = (cmd.speed + cmd.steer)*(float)MAX_SPEED; // Lewa strona: prędkość + skręt
    Command.dutyR = (cmd.speed - cmd.steer)*(float)MAX_SPEED; // Prawa strona: prędkość - skręt
    Vesc.setDuty(Command.dutyR);       
    Vesc.setDuty(Command.dutyL, 2);
}

bool VescDriver::update() {
    // Tutaj można dodać kod do aktualizacji stanu kierowcy, np. odczyt danych z VESC
    bool state = Vesc.getVescValues();
    dataR = Vesc.data; // Zakładam, że Vesc.data jest aktualizowane przez getVescValues() 
    state = state && Vesc.getVescValues(2);
    dataL = Vesc.data; // Zakładam, że Vesc.data jest aktualizowane przez getVescValues() 
    return state;
}



