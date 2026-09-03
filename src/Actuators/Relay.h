#pragma once

class Relay{

int LED_PIN;

public:

  Relay(int pin = -1){
    setup(pin);
  }

  void setup(int pin){
    LED_PIN = pin;
    pinMode(pin, OUTPUT);
    off();
  }
  
  void set(int pwm) { analogWrite(LED_PIN,  pwm) ; Serial.println("Relay " + String(LED_PIN) + " ON!"); }
  void on()         { digitalWrite(LED_PIN, LOW) ; Serial.println("Relay " + String(LED_PIN) + " ON!"); }
  void off()        { digitalWrite(LED_PIN, HIGH); Serial.println("Relay " + String(LED_PIN) + " ON!"); }
};




