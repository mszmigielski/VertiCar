#pragma once
#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include "GlobalTypes.h"
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
class WifiController {
private:
    // Tutaj przechowujemy najświeższe dane odebrane z przeglądarki
    HoverboardCommand _currentCommand = {0, 0}; // Domyślnie 0 prędkości i 0 skrętu
    WebSocketsServer _webSocket = WebSocketsServer(81); // Inicjalizacja serwera WebSocket na porcie 81
    unsigned long _lastPacketTime = 0; // Czas ostatniego odebranego pakietu

    void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    public:
    // Getter zwracający stałą referencję (szybko i bezpiecznie!)
    const HoverboardCommand& getCommand() const { 
        return _currentCommand; 
    }

    // Metoda, która uruchomi naszą sieć
    void initAP(const char* ssid, const char* password);
    void loop();
};