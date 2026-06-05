#include "Pidcontroler.h"

// Konstruktor - inicjalizuje zmienne i wgrywa ustawienia początkowe
PidControler::PidControler(float kp, float ki, float kd, float tau) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _tau = tau;
    
    
    _outMin = -1.0f; 
    _outMax =  1.0f;

    reset(); // Zerowanie zmiennych stanu
}

// Zmiana nastaw w locie (np. z menu OLED lub przez Wi-Fi)
void PidControler::setTunings(float kp, float ki, float kd) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
}

// Ustawienie fizycznych limitów urządzenia wykonawczego
void PidControler::setLimits(float min, float max) {
    _outMin = min;
    _outMax = max;
}

// Pełny reset pamięci regulatora
void PidControler::reset() {
    _integralSum = 0.0f;
    _prevMeasurement = 0.0f;
    _prevDerivative = 0.0f;
    _isFirstRun = true;
}

// GŁÓWNA LOGIKA REGULATORA
float PidControler::update(float setpoint, float measurement, float dt) {
    // Zabezpieczenie przed dzieleniem przez zero w przypadku błędu czasu
    if (dt <= 0.0f) {
        return 0.0f; 
    }

    // Jeśli to pierwsze wywołanie po resecie, synchronizujemy pomiar, 
    // aby zapobiec gigantycznemu szarpnięciu członu D na starcie.
    if (_isFirstRun) {
        _prevMeasurement = measurement;
        _isFirstRun = false;
    }

    // 1. Wyliczenie błędu
    float error = setpoint - measurement;

    // 2. Człon Proporcjonalny (P)
    float P = _kp * error;

    // 3. Człon Różniczkujący (D) z filtrem dolnoprzepustowym
    // Różniczkujemy po POMIARZE, nie po błędzie (Derivative on Measurement)
    float deltaMeasurement = measurement - _prevMeasurement;
    float rawDerivative = -(deltaMeasurement / dt);
     Serial.print(rawDerivative);//---------------------------------------------------------
    
    // Aplikacja filtru LPF do wygładzenia szumu z czujnika IMU
    float D = _prevDerivative + (dt / (_tau + dt)) * (rawDerivative - _prevDerivative);
    Serial.print("\t"); Serial.println(D); //-------------------------------------------------------------
    // 4. Człon Całkujący (I) - krok narastania
    float I_step = _ki * error * dt;

    // 5. Wstępne zsumowanie całego wyjścia regulatora
    float output = P + _integralSum + I_step + (_kd * D);

    // 6. Anti-Windup (Dynamiczne bindowanie / Clamping)
    if (output > _outMax) {
        output = _outMax;
        // Jesteśmy na limicie. Pozwalamy całce się zaktualizować TYLKO
        // jeśli błąd ma znak ujemny (czyli całka będzie maleć, a nie dalej rosnąć)
        if (error < 0.0f) {
            _integralSum += I_step;
        }
    } 
    else if (output < _outMin) {
        output = _outMin;
        // Jesteśmy na dolnym limicie. Pozwalamy całce rosnąć (błąd dodatni)
        if (error > 0.0f) {
            _integralSum += I_step;
        }
    } 
    else {
        // Normalna praca, wyjście nie uderza w limity, aktualizujemy całkę
        _integralSum += I_step;
    }

    // 7. Zapisanie stanów na poczet kolejnego wywołania funkcji
    _prevMeasurement = measurement;
    _prevDerivative = D;

    return output;
}