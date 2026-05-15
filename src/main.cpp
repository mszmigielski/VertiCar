#include <Arduino.h>

// --- KONFIGURACJA UART ---
#define HOVER_SERIAL_BAUD   115200
#define START_FRAME         0xABCD

// Piny dla ESP32 (Możesz je zmienić na dowolne wolne piny GPIO w Twoim S3)
#define HOVER_RX_PIN 8  // Podłącz do TX płyty Hoverboarda
#define HOVER_TX_PIN 18  // Podłącz do RX płyty Hoverboarda

// Tworzymy obiekt sprzętowego portu szeregowego nr 1
HardwareSerial HoverSerial(1);

// --- STRUKTURY DANYCH ---
typedef struct {
   uint16_t start;
   int16_t  steer;
   int16_t  speed;
   uint16_t checksum;
} __attribute__((packed)) SerialCommand;

typedef struct {
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;
} __attribute__((packed)) SerialFeedback;

// --- ZMIENNE GLOBALNE ---
SerialCommand Command;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

// Zmienne do parsowania
uint8_t idx = 0;
uint16_t bufStartFrame;
byte *p;
byte incomingByte;
byte incomingBytePrev;

// Zmienne czasowe
unsigned long iTimeSend = 0;
const int timeSendDelay = 50; // 50ms = 20Hz

// Zmienne testowe
int testSpeed = 0;
int testSteer = 0;

void setup() {
  // Inicjalizacja portu USB do debugowania na komputerze
  Serial.begin(9600);
  
  // Inicjalizacja sprzętowego UART do komunikacji z hoverboardem
  HoverSerial.begin(HOVER_SERIAL_BAUD, SERIAL_8N1, HOVER_RX_PIN, HOVER_TX_PIN);
  
  Serial.println("START: Inicjalizacja sprzetowego UART dla ESP32 zakończona...");
  Serial.println("Pamiętaj o podniesieniu kół robota!");
}

void SendCommand() {
  Command.start    = (uint16_t)START_FRAME;
  Command.steer    = testSteer;
  Command.speed    = testSpeed;
  Command.checksum = (uint16_t)(Command.start ^ Command.steer ^ Command.speed);

  // Wysłanie paczki bajtów do płyty przez sprzętowy port
  HoverSerial.write((uint8_t *) &Command, sizeof(Command)); 
}

void ReceiveFeedback() {
  if (HoverSerial.available()) {
    incomingByte = HoverSerial.read();
    bufStartFrame = ((uint16_t)(incomingByte) << 8) | incomingBytePrev;
  } else {
    return;
  }

  if (bufStartFrame == START_FRAME) {                     
    p       = (byte *)&NewFeedback;
    *p++    = incomingBytePrev;
    *p++    = incomingByte;
    idx     = 2;
  } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  
    *p++    = incomingByte;
    idx++;
  }

  if (idx == sizeof(SerialFeedback)) {
    uint16_t checksum;
    checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ 
                          NewFeedback.speedR_meas ^ NewFeedback.speedL_meas ^ 
                          NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);

    if (NewFeedback.checksum == checksum) {
      memcpy(&Feedback, &NewFeedback, sizeof(SerialFeedback));

      // Wypisz dane z hoverboarda
      float voltage = (float)Feedback.batVoltage / 100.0;
      float temp = (float)Feedback.boardTemp / 10.0;
      
      Serial.print("Bat: "); Serial.print(voltage); Serial.print("V | ");
      Serial.print("Temp: "); Serial.print(temp); Serial.print("C | ");
      Serial.print("RPM_L: "); Serial.print(Feedback.speedL_meas); Serial.print(" | ");
      Serial.print("RPM_R: "); Serial.println(Feedback.speedR_meas);
      
    } else {
      Serial.println("OSTRZEŻENIE: Błąd sumy kontrolnej!");
    }
    idx = 0; 
  }

  incomingBytePrev = incomingByte;
}

void loop() {
  unsigned long timeNow = millis();

  // Test ruchowy: Przód -> Stop -> Tył -> Stop (co 2 sekundy)
  int cycle = (timeNow / 2000) % 4; 
  
  if (cycle == 0) {
    testSpeed = 50;
  } else if (cycle == 1) {
    testSpeed = 0;
  } else if (cycle == 2) {
    testSpeed = -50;
  } else {
    testSpeed = 0;
  }

  // Nasłuch i wysyłanie
  ReceiveFeedback();

  if (timeNow - iTimeSend > timeSendDelay) {
    iTimeSend = timeNow;
    SendCommand();
  }
}