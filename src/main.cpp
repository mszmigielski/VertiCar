#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "HoverDriver.h"
#include "WifiController.h"

// Definicje globalnych obiektów
 // Używamy sprzętowego UART1
WifiController wifiController;
HoverDriver hoverDriver(Serial1);
void setup() {
  // Inicjalizacja portu USB do debugowania na komputerze
  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.println("Uruchamianie VertiCar...");

  wifiController.initAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Punkt dostępowy WiFi uruchomiony.");

  Serial1.begin(HOVER_SERIAL_BAUD, SERIAL_8N1, HOVER_RX_PIN, HOVER_TX_PIN);
  Serial.println("Port szeregowy do Hoverboarda uruchomiony.");
}


void loop() {
  wifiController.loop();
  SpeedCommand com;
  static uint32_t lastPrintMs = 0;
  if (millis() - lastPrintMs >= 50) { // 20 Hz
    lastPrintMs = millis();
    const SpeedCommand cmd = wifiController.getCommand();
    com = cmd; // Kopiujemy do lokalnej zmiennej, by mieć stabilne dane podczas debugowania
    //Serial.print(cmd.speed); 
    //Serial.println(cmd.steer);
  }

  
  hoverDriver.move(com);
  //Serial.println("Test move forward");

  //delay(1); // daje czas schedulerowi/USB/WiFi
  
}