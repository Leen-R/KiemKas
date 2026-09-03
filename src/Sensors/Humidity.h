#pragma once

#include <SensirionI2cSht4x.h>

#define NO_ERROR 0

SensirionI2cSht4x sht4x;

static char errorMessage[64];
static int16_t error;

class SHT40{
public: 

float aTemperature;
float aHumidity;

void setup() {

    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    sht4x.begin(Wire, SHT40_I2C_ADDR_44);

    sht4x.softReset();
    delay(10);
    uint32_t serialNumber = 0;
    error = sht4x.serialNumber(serialNumber);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute serialNumber(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("serialNumber: ");
    Serial.print(serialNumber);
    Serial.println();
}

void update() {

    error = sht4x.measureLowestPrecision(aTemperature, aHumidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute measureLowestPrecision(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("aTemperature: ");
    Serial.print(aTemperature);
    Serial.print("\t");
    Serial.print("aHumidity: ");
    Serial.print(aHumidity);
    Serial.println();
}
};