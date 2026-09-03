#pragma once

class Switch{

  int SWITCH_PIN;

public:

  Switch(int pin = -1){
    setup(pin);
  }

  void setup(int pin){
    SWITCH_PIN = pin;
    pinMode(pin, OUTPUT);
  }

  void on() { digitalWrite(SWITCH_PIN, HIGH); Serial.println("Switch " + String(SWITCH_PIN) + " ON!"); }
  void off(){ digitalWrite(SWITCH_PIN, LOW);  Serial.println("Switch " + String(SWITCH_PIN) + " OFF!");}
};





