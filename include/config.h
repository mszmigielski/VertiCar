#pragma once


// Konfiguracja dla sterownika Hoverboard
#define HOVER_SERIAL_BAUD   115200
#define HOVER_RX_PIN 8  // Podłącz do TX płyty Hoverboarda
#define HOVER_TX_PIN 18  // Podłącz do RX płyty Hoverboarda
#define START_FRAME         0xABCD
#define MAX_SPEED           50 // Maksymalna prędkość (dostosuj do potrzeb)

// Konfiguracja dla WiFi
#define WIFI_SSID "VertiCar_AP"
#define WIFI_PASSWORD "verticar123"