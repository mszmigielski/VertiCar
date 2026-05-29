#include "VescUart.h"

// Tworzymy obiekt obsługujący VESC
VescUart Vesc;

void setup() {
  // Port Serial0 (USB) do podglądu danych na komputerze (Monitor Portu Szeregowego)
  Serial.begin(9600);
  while (!Serial) { ; } // Czekaj na otwarcie monitora

  // Port Serial2 do komunikacji z VESC (dla ESP32 piny RX=16, TX=17, dla Mega piny RX=17, TX=16)
  // Jeśli używasz Arduino Mega, wpisz po prostu: Serial2.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); 

  // Powiązanie biblioteki z fizycznym portem Serial2
  Vesc.setSerialPort(&Serial2);
}

void loop() {

uint8_t id = 2;
  // Inne dostępne metody sterowania (używaj tylko jednej na raz!):
   Vesc.setDuty(0.2);       // Sterowanie wypełnieniem (0.2 = 20% napięcia)
   Vesc.setDuty(0.2, id);       // Sterowanie wypełnieniem (0.2 = 20% napięcia)
  // Vesc.setRPM(1500);       // Sterowanie prędkością (w ERPM)
  // Vesc.setBrakeCurrent(5); // Hamowanie prądem 5A

  delay(20); // Krótka przerwa, optymalna częstotliwość pętli to 20-50Hz

  // 2. ODBIERANIE TELEMETRII (DANYCH Z VESC)
  // Funkcja getVescValues() pyta VESC o aktualne parametry i zwraca true, jeśli odpowiedź dotarła
  if ( Vesc.getVescValues() ) {
    Serial.println("--- DANE Z VESC ---");
    
    // Obroty elektryczne (ERPM)
    Serial.print("Prędkość (ERPM): ");
    Serial.println(Vesc.data.rpm);
    
    // Aktualne napięcie na baterii
    Serial.print("Napięcie zasilania: ");
    Serial.print(Vesc.data.inpVoltage);
    Serial.println(" V");
    
    // Prąd pobierany z akumulatora
    Serial.print("Prąd z baterii: ");
    Serial.print(Vesc.data.avgInputCurrent);
    Serial.println(" A");

    // Temperatura sterownika
    Serial.print("Temperatura MOSFETów: ");
    Serial.print(Vesc.data.tempMosfet);
    Serial.println(" C");
  } else {
    Serial.println("Błąd komunikacji! Sprawdź przewody UART.");
  }

  delay(200); // Pobieraj dane rzadziej, np. co 200ms, żeby nie zapychać portu szeregowego
}