#pragma once
#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "HoverDriver.h"
// --- ZMIENNE GLOBALNE ---
HardwareSerial HoverSerial(1); // Używamy sprzętowego UART1

void setup() {
  // Inicjalizacja portu USB do debugowania na komputerze
  Serial.begin(9600);
  
  // Inicjalizacja sprzętowego UART do komunikacji z hoverboardem
  HoverSerial.begin(HOVER_SERIAL_BAUD, SERIAL_8N1, HOVER_RX_PIN, HOVER_TX_PIN);
  HoverDriver Motor(HoverSerial);

}





void loop() {
 
}