#include "VescUart.h"
#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "WifiController.h"
#include "MPU9250.h"
#include <Wire.h>
#include "VescDriver.h"
#include "Imu.h"
#include "ControlPanel.h"
//TODO: uporządkować includy


VescDriver vescDriver(Serial2);
WifiController wifiController;
Imu imu(Wire);
ControlPanel panel(WHITE_BUTTON_PIN, WHITE_BUTTON_LED_PIN);
unsigned long previousMillis = 0;




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
  
  previousMillis = millis();

  // Inicjalizacja imu
  Wire.begin(IMU_SDA_PIN, IMU_SCL_PIN);
  delay(1000);
  if (!imu.init()) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }


  panel.init();
}

void loop() {
  panel.update(); // Update the control panel state
  wifiController.loop(); // Obsługa WiFi i WebSocketów
  SpeedCommand s_cmd = wifiController.getCommand(); // Pobierz aktualne polecenie prędkości i skrętu
  vescDriver.move(s_cmd); // Wyślij polecenie do VESC

  imu.update(); // Aktualizacja danych z IMU
  float pitch = imu.getPitch(); // Przykładowe pobranie kąta pitch (możesz też pobrać yaw i roll)
  wifiController.sendTelemetry(pitch); // Wyślij dane telemetryczne do przeglądarki

}