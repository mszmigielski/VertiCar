#pragma once



#define MAX_DUTY           0.2 // Maksymalna prędkość (dostosuj do potrzeb)
#define MAX_CURRENT        5.0 // Maksymalny prąd (dostosuj do potrzeb)
#define MAX_ON_CURRENT_DUTY           0.3 // Maksymalne obroty silnika (dostosuj do potrzeb)  
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