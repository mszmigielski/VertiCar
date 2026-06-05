#pragma once


// Konfiguracja dla sterownika Hoverboard
#define HOVER_SERIAL_BAUD   115200
#define HOVER_RX_PIN 8  // Podłącz do TX płyty Hoverboarda
#define HOVER_TX_PIN 18  // Podłącz do RX płyty Hoverboarda
#define START_FRAME         0xABCD
#define MAX_SPEED           0.2 // Maksymalna prędkość (dostosuj do potrzeb)

// Konfiguracja dla WiFi
#define WIFI_SSID "VertiCar_AP"
#define WIFI_PASSWORD "verticar123"


//IMU
#define MPU9250_ADDRESS 0x68
#define IMU_SDA_PIN 4
#define IMU_SCL_PIN 5

// Interface box
#define WHITE_BUTTON_PIN 7
#define WHITE_BUTTON_LED_PIN 6