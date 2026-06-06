#pragma once
#include <Arduino.h>
class PidControler {
private:
    // --- Nastawy regulatora ---
    float _kp;
    float _ki;
    float _kd;
    
    // --- Parametry filtru i limity ---
    float _tau;     // Stała czasowa filtru dolnoprzepustowego dla członu D
    float _outMin = -1.0f;  // Minimalne wyjście (np. -1.0 dla VESC)
    float _outMax = 1.0f;  // Maksymalne wyjście (np. 1.0 dla VESC)

    // --- Pamięć stanów wewnętrznych (Memory) ---
    float _integralSum;      // Skumulowana suma członu I
    float _prevMeasurement;  // Poprzedni odczyt czujnika (do różniczkowania)
    float _prevDerivative;   // Ostatnia przefiltrowana wartość różniczki
    bool  _isFirstRun;       // Flaga pierwszego uruchomienia (dla gładkiego startu)

public:
    // Konstruktor
    PidControler(float kp, float ki, float kd, float tau);

    // Konfiguracja (można wywoływać w locie)
    void setTunings(float kp, float ki, float kd);
    void setLimits(float min, float max);

    // Główna funkcja wyliczeniowa (wywoływana w pętli)
    float update(float setpoint, float measurement, float dt);

    // Resetowanie pamięci regulatora (E-Stop, podniesienie robota)
    void reset();
};