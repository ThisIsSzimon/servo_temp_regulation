#include <Arduino.h>
#include <WiFi.h>
#include "modbus_hdl.h"
#include "web_srv.h"
#include "servo_ctl.h"

const char* ssid = "Szymon Komputer";
const char* password = "70wf-lrta-dfna";

unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi OK!");
    Serial.print("Adres HMI: http://");
    Serial.println(WiFi.localIP());

    setupModbus();
    setupWebServer();
    setupServo();
}

void loop() {
    if (millis() - lastUpdate > 2000) {
        float t = readTemperature();
        if (t > -100.0) {
            updateTemperatureHMI(t);
        }
        lastUpdate = millis();
    }

    handleServoSerial();
}