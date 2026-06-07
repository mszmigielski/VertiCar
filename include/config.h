#pragma once



#define MAX_DUTY           0.2 // Maksymalna prędkość (dostosuj do potrzeb)
#define MAX_CURRENT        5.0 // Maksymalny prąd (dostosuj do potrzeb)
#define MAX_ON_CURRENT_DUTY           0.4 // Maksymalne obroty silnika (dostosuj do potrzeb)  
#define MAX_RPM             2000 // Maksymalne obroty silnika (dostosuj do potrzeb)

#define Tau  0.1
#define Kpa  0.09
#define Kia  0.000
#define Kda  0.008

#define EQULIBIRUM_ANGLE 4.5
#define MAX_ANGLE 25 // Maksymalny kąt pochylenia, przy którym robot może się poruszać (dostosuj do potrzeb)
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