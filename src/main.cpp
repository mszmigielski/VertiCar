#include "VescUart.h"
#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include "WifiControler.h"
#include "MPU9250.h"
#include <Wire.h>
#include "VescDriver.h"
#include "Imu.h"
#include "ControlPanel.h"
#include "PidControler.h"
//TODO: uporządkować includy


VescDriver vescDriver(Serial2);
WifiControler wificontroler;
Imu imu(Wire);
ControlPanel panel(WHITE_BUTTON_PIN, WHITE_BUTTON_LED_PIN);
unsigned long previousMillis = 0;
PIDTunings pidTunings; 
float kpa = Kpa, kia = Kia, kda = Kda, ta = Tau;
PidControler PIDangle(kpa, kia, kda, ta);
PidControler PIDspeed(0,0,0,0.05f);
unsigned long timer = millis();
float angle = EQULIBIRUM_ANGLE;
float accAngle = 0.0f;
SpeedCommand joy = {0.0f, 0.0f}; // Przykładowe polecenie prędkości (speed, steer)
SpeedCommand currentSpeed = {0.0f, 0.0f}; // Przykładowe polecenie prędkości (speed, steer)
SpeedCommand s_cmd = {0.0f, 0.0f}; // Przykładowe polecenie prędkości (speed, steer)

void setup() {
  // Port Serial0 (USB) do podglądu danych na komputerze (Monitor Portu Szeregowego)
   Serial.begin(115200);
 // while (!Serial) { delay(10); }
  Serial.println("Uruchamianie VertiCar...");

  wificontroler.initAP(WIFI_SSID, WIFI_PASSWORD);
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
  panel.update();
  static bool state = false;


  PIDTunings nastawy = wificontroler.getPidTunings();
  //PIDangle.setTunings(nastawy.kp, nastawy.ki, nastawy.kd);
  PIDangle.setTunings(nastawy.kp, nastawy.ki, nastawy.kd);
  wificontroler.loop();
  joy = wificontroler.getCommand(); 
  currentSpeed.speed = vescDriver.dataL.rpm + vescDriver.dataR.rpm/2; 
  imu.update(); 

  float pitch = imu.getPitch();
  float current = vescDriver.dataR.avgMotorCurrent + vescDriver.dataL.avgMotorCurrent; 


  float dt = (millis() - previousMillis) / 1000.0f; 
  previousMillis = millis();

  //accAngle =4 * PIDspeed.update(joy.speed*MAX_RPM, currentSpeed.speed, dt); //Maksymalny kąt pochylenia 4 stopnie.
  accAngle = joy.speed*2;
  s_cmd.speed = PIDangle.update(angle + accAngle, pitch, dt);
  s_cmd.steer = joy.steer*0.1f; // Przykładowe przypisanie skrętu (można rozbudować o PID dla skrętu)

  if(!panel.isWhiteButtonPressed() && wificontroler.getButton0() == 1 && abs(pitch) < MAX_ANGLE) { // Jeśli przycisk jest wciśnięty, zmień stan
    vescDriver.setCurrent(s_cmd);
  } else {
    SpeedCommand stopCmd = {0.0f, 0.0f};
    vescDriver.setCurrent(stopCmd);
    //Serial.println("Zatrzymano silniki");
    //vescDriver.setBrakeCurrent(5.0f);
    PIDangle.reset();
    PIDspeed.reset();
  }


//Serial.println(wificontroler.getButton0());
  if (millis() - timer > 50){
        state = vescDriver.update(); // Update VESC state (read data, etc.)
    wificontroler.sendTelemetry(pitch, current, (int)(dt * 1000)); // Wyślij dane telemetryczne do przeglądarki
    timer = millis();  
  }
  
}