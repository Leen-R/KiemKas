#pragma once

// Default placeholder for unassigned/inactive pins
#define NO_PIN -1

// ==========================================
// ACTUATOR PINS
// ==========================================
// Fan
#define PIN_FAN_1         17

// Heater
#define PIN_HEATER        15
#define PIN_HEATER_FAN    NO_PIN

// Humidifier
#define PIN_HUMIDIFIER    5

// Water Pump
#define PIN_WATERPUMP     18

// ==========================================
// SENSOR PINS
// ==========================================
// 1-Wire Temperature Sensor (DS18B20)
#define PIN_ONE_WIRE_BUS  25

// I2C Pins (Default ESP32 pins are SDA=21, SCL=22)
// Devices: ADS1115 (0x48), SHT40 (0x44)