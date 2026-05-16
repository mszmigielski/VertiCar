#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "HoverDriver.h"
#include "WifiController.h"

// --- ZMIENNE GLOBALNE ---
//HardwareSerial HoverSerial(1); // Używamy sprzętowego UART1
WifiController wifiController;

void setup() {
  // Inicjalizacja portu USB do debugowania na komputerze
  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.println("Uruchamianie VertiCar...");
  wifiController.initAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Punkt dostępowy WiFi uruchomiony.");
}


void loop() {
  wifiController.loop();

  static uint32_t lastPrintMs = 0;
  if (millis() - lastPrintMs >= 50) { // 20 Hz
    lastPrintMs = millis();
    const SpeedCommand cmd = wifiController.getCommand();
    Serial.println(cmd.speed);
  }

  delay(1); // daje czas schedulerowi/USB/WiFi
}