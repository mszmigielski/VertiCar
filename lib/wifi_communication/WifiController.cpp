#pragma once
#include <Arduino.h>
#include "config.h"
#include "GlobalTypes.h"
#include <WiFi.h>
#include "WifiController.h"
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

void WifiController::initAP(const char*ssid, const char* password) {
    WiFi.softAP(ssid, password);
    _webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        this->onWebSocketEvent(num, type, payload, length);
    });
}


void WifiController::onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * palyload, size_t length){
    switch(type) {
        case WStype_DISCONNECTED:
        _currentCommand = {0, 0}; // Resetuj komendę, gdy klient się rozłączy
        case WStype_CONNECTED:
        // to do: dodać intykador w postaci diody
        break;
        case WStype_TEXT:
        _lastPacketTime = millis(); // Aktualizuj czas ostatniego pakietu

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, palyload);
        if (!error) {
            const char* t = doc["t"];

            if (!error) {
                const char* t = doc["t"]; // Sprawdzamy typ wiadomości
                
                if (t != nullptr && strcmp(t, "ctrl") == 0) {
                    _currentCommand.speed = doc["x"];
                    _currentCommand.steer = doc["y"];
                } 
                else if (t != nullptr && strcmp(t, "stop") == 0) {
                    _currentCommand.speed = 0;
                    _currentCommand.steer = 0;
                }
            }
            break;
        }
    }
}




void WifiController::loop() {
    _webSocket.loop();
    //watchdog
    if (millis() - _lastPacketTime > 500) { // Jeśli minęło więcej niż 5 sekund od ostatniego pakietu
        _currentCommand = {0, 0}; // Resetuj komendę do bezpiecznej wartości
    }
}