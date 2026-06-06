#pragma once
#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include "GlobalTypes.h"
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <LittleFS.h>

class WifiControler {
private:
    // Tutaj przechowujemy najświeższe dane odebrane z przeglądarki
    SpeedCommand _currentCommand = {0, 0}; // Domyślnie 0 prędkości i 0 skrętu
    PIDTunings _currentPidTunings = {0, 0, 0}; // Domyślne nastawy PID (do uzupełnienia)
    WebSocketsServer _webSocket{81}; 
    WebServer _httpServer{80};
     unsigned long _lastPacketTime = 0; // Czas ostatniego odebranego pakietu

    void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

public:
    
    const SpeedCommand& getCommand() const { 
        return _currentCommand; 
    }

    const PIDTunings& getPidTunings() const {
        return _currentPidTunings;
    }

    void initAP(const char* ssid, const char* password);
    void loop();
    void sendTelemetry(float angle, float current, int loopTime);
};