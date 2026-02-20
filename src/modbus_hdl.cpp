#include "modbus_hdl.h"
#include <ModbusRTU.h>

ModbusRTU mb;

void setupModbus() {
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    mb.begin(&Serial2);
    mb.master();
}

float readTemperature() {
    uint16_t res[1] = {0};
    float temp = -999.0;
    
    // Wysyłamy żądanie odczytu (Adres 1, Rejestr 1, 1 słowo)
    mb.readHreg(1, 0x0001, res, 1);
    
    // Zamiast szukać funkcji stanu, po prostu wykonujemy taski przez chwilę,
    // aby biblioteka miała czas odebrać ramkę z RS485.
    unsigned long start = millis();
    while (millis() - start < 150) { // 150ms to wystarczający czas na odpowiedź przy 9600 bodach
        mb.task();
        yield(); // Pozwól ESP32 obsłużyć procesy w tle (WiFi)
    }

    // Jeśli w tablicy pojawiło się coś innego niż zero
    if (res[0] != 0) {
        temp = (float)res[0] / 10.0;
    }
    
    return temp;
}