#include <Arduino.h>
#include <ESP32Servo.h>

Servo myServo;

const int servoPin = 18;
int currentAngle = 0;  // startowa pozycja

void setupServo() {
    myServo.setPeriodHertz(50);      // standard 50 Hz
    myServo.attach(servoPin, 500, 2400); 
    Serial.println("Serwo gotowe. Podaj kat 0-180:");
}

void handleServoSerial() {
    if (Serial.available()) {

        int target = Serial.parseInt();
        target = constrain(target, 0, 180);

        Serial.print("Ruch do: ");
        Serial.println(target);

        if (target > currentAngle) {
            for (int pos = currentAngle; pos <= target; pos++) {
                myServo.write(pos);
                delay(20);   // im większy delay → wolniej
            }
        } else {
            for (int pos = currentAngle; pos >= target; pos--) {
                myServo.write(pos);
                delay(20);
            }
        }

        currentAngle = target;

        while (Serial.available()) Serial.read();
    }
}