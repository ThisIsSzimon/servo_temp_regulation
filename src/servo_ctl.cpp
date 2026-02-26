#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_ctl.h"

Servo myServo;

const int servoPin = 18;

enum Mode { AUTO_MODE, MANUAL_MODE };
Mode currentMode = AUTO_MODE;

int currentAngle = 90;
int targetAngle = 90;

unsigned long lastStepTime = 0;
const int stepDelay = 15;   // im większe tym wolniej
const int stepSize = 1;

void setupServo() {

    myServo.setPeriodHertz(50);
    myServo.attach(servoPin, 500, 2400);   // Twoje sprawdzone parametry

    myServo.write(90);
    currentAngle = 90;
    targetAngle = 90;

    Serial.println("Start: TRYB AUTO");
    Serial.println("Wpisz 'a' = AUTO, 'm' = MANUAL");
}

void handleServoSerial() {

    if (!Serial.available()) return;

    char c = Serial.read();

    // === ZMIANA TRYBU ===
    if (c == 'a' || c == 'A') {
        currentMode = AUTO_MODE;
        Serial.println("Przelaczono na TRYB AUTO");
        return;
    }

    if (c == 'm' || c == 'M') {
        currentMode = MANUAL_MODE;
        Serial.println("Przelaczono na TRYB MANUAL");
        return;
    }

    // === STEROWANIE MANUAL ===
    if (currentMode == MANUAL_MODE) {

        // jeśli pierwszy znak jest cyfrą, czytaj dalej liczbę
        if (isDigit(c)) {

            String input = "";
            input += c;

            delay(5); // krótka pauza na resztę znaków

            while (Serial.available()) {
                char next = Serial.read();
                if (isDigit(next))
                    input += next;
            }

            int val = input.toInt();
            val = constrain(val, 0, 180);

            targetAngle = val;

            Serial.print("Manual target: ");
            Serial.println(targetAngle);
        }
    }
}

void setAutoTarget(float temperature) {

    if (currentMode != AUTO_MODE) return;

    if (temperature < 30.0) {
        targetAngle = 90;
    }
    else if (temperature > 50.0) {
        targetAngle = 0;
    }
}

void updateServo() {

    if (millis() - lastStepTime < stepDelay) return;
    lastStepTime = millis();

    if (currentAngle < targetAngle) {
        currentAngle += stepSize;
        myServo.write(currentAngle);
    }
    else if (currentAngle > targetAngle) {
        currentAngle -= stepSize;
        myServo.write(currentAngle);
    }
}