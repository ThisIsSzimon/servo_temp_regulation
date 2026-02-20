#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "web_srv.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Funkcja wysyłająca dane do wszystkich podłączonych przeglądarek
void updateTemperatureHMI(float temp) {
    // Wysyłamy temperaturę jako tekst, np. "23.5"
    // textAll rozsyła to do każdego, kto ma otwartą stronę
    ws.textAll(String(temp, 1));
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket: Klient #%u połączony\n", client->id());
    }
}

void setupWebServer() {
    // Inicjalizacja pamięci plików
    if(!LittleFS.begin(true)){
        Serial.println("Błąd LittleFS!");
        return;
    }

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    // Serwowanie plików statycznych z folderu /data
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    // Obsługa błędu 404
    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Nie znaleziono pliku. Czy wgrałeś Filesystem Image?");
    });

    server.begin();
    Serial.println("Serwer HTTP uruchomiony!");
}