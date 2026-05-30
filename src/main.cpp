#include "VescUart.h"
#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "WifiController.h"

// Tworzymy obiekt obsługujący VESC
VescUart Vesc;
WifiController wifiController;
unsigned long previousMillis = 0;

void move(SpeedCommand s_cmd) {
    uint8_t id = 2;
  VescCommand cmd = {
    .dutyL = (s_cmd.speed + s_cmd.steer)*(float)MAX_SPEED, // Lewa strona: prędkość + skręt
    .dutyR = (s_cmd.speed - s_cmd.steer)*(float)MAX_SPEED  // Prawa strona: prędkość - skręt
    };
   Vesc.setDuty(cmd.dutyL);       
   Vesc.setDuty(cmd.dutyR, id);  
   if (previousMillis - millis() >= 500) { // Wyświetlaj komendę co 500ms, żeby nie zapychać portu szeregowego
        previousMillis = millis();
   Serial.print("Wysłano komendę: DutyL = "); Serial.print(cmd.dutyL); Serial.print(" | DutyR = "); Serial.println(cmd.dutyR);
   }

}


void setup() {
  // Port Serial0 (USB) do podglądu danych na komputerze (Monitor Portu Szeregowego)
   Serial.begin(115200);
 // while (!Serial) { delay(10); }
  Serial.println("Uruchamianie VertiCar...");

  wifiController.initAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Punkt dostępowy WiFi uruchomiony.");

  // Port Serial2 do komunikacji z VESC (dla ESP32 piny RX=16, TX=17, dla Mega piny RX=17, TX=16)
  // Jeśli używasz Arduino Mega, wpisz po prostu: Serial2.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 40, 39); 

  // Powiązanie biblioteki z fizycznym portem Serial2
  Vesc.setSerialPort(&Serial2);
  previousMillis = millis();
}

void loop() {

  wifiController.loop(); // Obsługa WiFi i WebSocketów
  SpeedCommand s_cmd = wifiController.getCommand(); // Pobierz aktualne polecenie prędkości i skrętu
  //Serial.print(s_cmd.speed); Serial.print(" | "); Serial.println(s_cmd.steer); // Debug: wyświetl otrzymane polecenie
  move(s_cmd); // Wyślij polecenie do VESC
       


/*
  if ( Vesc.getVescValues() ) {
    // Obroty elektryczne (ERPM)
    Serial.print("Prędkość (ERPM): ");
    Serial.println(Vesc.data.rpm);
    
  } else {
    //Serial.println("Błąd komunikacji! Sprawdź przewody UART.");
  }
*/
  //delay(200); // Pobieraj dane rzadziej, np. co 200ms, żeby nie zapychać portu szeregowego
}