#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// Ustawienia sieci Wi-Fi tworzonej przez ESP32
const char* ssid = "ESP32_Gamepad";
const char* password = ""; // Sieć otwarta, bez hasła

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Zmienne do przechowywania odczytów joysticka (do późniejszego użycia np. dla silników)
float joyX = 0.0;
float joyY = 0.0;

// ==============================================================================
// KOD STRONY HTML I JAVASCRIPT (Wysyłany do przeglądarki na laptopie)
// ==============================================================================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32 Gamepad Test</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; background: #222; color: #fff; }
    h1 { color: #00d2ff; }
    #status { font-size: 20px; color: #ff3333; }
    .joystick-data { font-size: 30px; margin-top: 20px; font-family: monospace; }
  </style>
</head>
<body>
  <h1>Panel Sterowania ESP32</h1>
  <p>Podłącz pada do komputera i rusz joystickiem, aby obudzić Gamepad API.</p>
  <p id="status">Brak połączenia WebSocket</p>
  <div class="joystick-data">X: <span id="valX">0.00</span> | Y: <span id="valY">0.00</span></div>

  <script>
    // Łączenie z ESP32 przez WebSocket na porcie 81
    var ws = new WebSocket('ws://' + window.location.hostname + ':81/');
    var statusEl = document.getElementById('status');
    var lastSend = 0;

    ws.onopen = function() { 
        statusEl.innerText = 'Połączono z ESP32 (WebSocket aktywny)!'; 
        statusEl.style.color = '#33ff33';
    };
    ws.onclose = function() { 
        statusEl.innerText = 'Rozłączono...'; 
        statusEl.style.color = '#ff3333';
    };

    function gameLoop() {
      var gamepads = navigator.getGamepads();
      var gp = gamepads[0]; // Bierzemy pierwszego podłączonego pada

      if (gp && ws.readyState === WebSocket.OPEN) {
        var now = Date.now();
        // Wysyłamy dane co 50ms (20 razy na sekundę), by nie zaciąć mikrokontrolera
        if (now - lastSend > 50) { 
          // Oś X to index 0, Oś Y to index 1 (lewy joystick)
          var x = gp.axes[0].toFixed(2);
          var y = gp.axes[1].toFixed(2);
          
          // Wysyłamy tylko jeśli wychylenie jest poza "martwą strefą" (deadzone 10%)
          // Oraz wysyłamy format tekstowy: "X,Y" np. "0.50,-0.85"
          if (Math.abs(x) > 0.1 || Math.abs(y) > 0.1) {
             ws.send(x + ',' + y);
          } else {
             // Gdy drążek wraca na środek, wysyłamy zera
             ws.send("0.00,0.00");
          }

          document.getElementById('valX').innerText = x;
          document.getElementById('valY').innerText = y;
          lastSend = now;
        }
      }
      requestAnimationFrame(gameLoop); // Pętla odświeżania przeglądarki
    }
    
    // Start pętli
    requestAnimationFrame(gameLoop);
  </script>
</body>
</html>
)rawliteral";

// ==============================================================================
// OBSŁUGA POŁĄCZEŃ WEBSOCKET (Odbieranie danych w ESP32)
// ==============================================================================
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
        // 1. Tworzymy bezpieczny bufor o 1 znak większy niż odebrana wiadomość
        char msg[length + 1];
        
        // 2. Kopiujemy do niego surowe dane
        memcpy(msg, payload, length);
        
        // 3. Sztucznie dodajemy znak końca (Null-terminator)
        msg[length] = '\0';

        // 4. Teraz bezpiecznie odczytujemy liczby zmiennoprzecinkowe
        if (sscanf(msg, "%f,%f", &joyX, &joyY) == 2) {
            Serial.print("Lewy Joy -> X: ");
            Serial.print(joyX);
            Serial.print(" | Y: ");
            Serial.println(joyY);
        }
    }
}

// ==============================================================================
// GŁÓWNA KONFIGURACJA
// ==============================================================================
void setup() {
    Serial.begin(9600);
    delay(2000); // Dajemy czas na otwarcie portu szeregowego

    Serial.println("\n--- Start Systemu ESP32 ---");

    // 1. Tworzenie sieci Wi-Fi (Access Point)
    WiFi.softAP(ssid, password);
    Serial.print("Siec Wi-Fi aktywna. Nazwa: ");
    Serial.println(ssid);
    Serial.print("Adres IP ESP32: ");
    Serial.println(WiFi.softAPIP());

    // 2. Konfiguracja serwera WWW
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", index_html);
    });
    server.begin();
    Serial.println("Serwer WWW uruchomiony na porcie 80.");

    // 3. Konfiguracja serwera WebSocket
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("Serwer WebSocket uruchomiony na porcie 81.");
}

void loop() {
    // Musimy regularnie wywoływać metody obsługi obu serwerów
    webSocket.loop();
    server.handleClient();
}