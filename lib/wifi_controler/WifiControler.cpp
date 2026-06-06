#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include <WiFi.h>
#include "WifiControler.h"
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>





void WifiControler::initAP(const char*ssid, const char* password) {

    if (!LittleFS.begin(true)) { 
        Serial.println("Błąd montowania systemu plików LittleFS!");
    } else {
        Serial.println("LittleFS zamontowany pomyślnie.");
    }

    WiFi.softAP(ssid, password);
    _lastPacketTime = millis();
    _webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        this->onWebSocketEvent(num, type, payload, length);
    });
    _webSocket.begin();

    _httpServer.on("/", [this](){
        if(LittleFS.exists("/index.html")) {
            File file = LittleFS.open("/index.html", "r");
            _httpServer.streamFile(file, "text/html");
            file.close();
        }
        else {
            _httpServer.send(404, "text/plain", "404 Not found");
        }      
    });
    _httpServer.begin();
}



void WifiControler::onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
    switch(type) {
        case WStype_DISCONNECTED:
        _currentCommand = {0, 0}; // Resetuj komendę, gdy klient się rozłączy
        break;
        case WStype_CONNECTED:
        // to do: dodać intykador w postaci diody
        _lastPacketTime = millis();
        break;
        case WStype_TEXT:
        _lastPacketTime = millis(); // Aktualizuj czas ostatniego pakietu
        
        // Odbicie ping
        if (length == 4 && memcmp(payload, "ping", 4) == 0) {
                _webSocket.sendTXT(num, "pong");
                break; 
            }

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
            break;
        }

        const char* t = doc["t"];
        if (t != nullptr && strcmp(t, "ctrl") == 0) {
            _currentCommand.speed = doc["y"].as<float>();
            _currentCommand.steer = doc["x"].as<float>();
        } 
        else if (t != nullptr && strcmp(t, "stop") == 0) {
            _currentCommand.speed = 0;
            _currentCommand.steer = 0;
        }
        else if (t != nullptr && strcmp(t, "TUNE_PID") == 0) {
            _currentPidTunings.kp = doc["p"].as<float>();
            _currentPidTunings.ki = doc["i"].as<float>();
            _currentPidTunings.kd = doc["d"].as<float>();
            // Tutaj możesz dodać kod do aktualizacji nastawów PID w swoim regulatorze
            
        }
        break;
        
    
    }
}




void WifiControler::loop() {
    _webSocket.loop();
    _httpServer.handleClient();
    //watchdog
    if (millis() - _lastPacketTime > 500) { // Jeśli minęło więcej niż 500 ms od ostatniego pakietu
        _currentCommand = {0, 0}; // Resetuj komendę do bezpiecznej wartości
    }
}


void WifiControler::sendTelemetry(float angle, float current, int loopTime) {
    static unsigned long lastTime = 0;
    if (millis() - lastTime < 100) { // Limituj wysyłanie danych telemetrycznych do co 100 ms
        return;
    }
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "{\"t\":\"tele\",\"ang\":%.2f,\"curr\":%.2f,\"loop\":%d}", angle, current, loopTime);

    _webSocket.broadcastTXT(buffer);
}