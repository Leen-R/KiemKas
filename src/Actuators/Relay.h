#pragma once

class Relay {
  int LED_PIN;
  bool state = false; 

public:
  Relay(int pin = NO_PIN) { setup(pin); }

  void setup(int pin) {
    LED_PIN = pin;
    if (LED_PIN != NO_PIN) {
      pinMode(LED_PIN, OUTPUT);
      off(); // Defaults to off
    }
  }
  
  void set(int pwm) { 
    if (LED_PIN != NO_PIN) { 
      analogWrite(LED_PIN, pwm); 
    }
  }

  void on() { 
    if (LED_PIN != NO_PIN && !state) { 
      digitalWrite(LED_PIN, LOW); 
      state = true;
      Serial.printf("Relay %d ON!\n", LED_PIN); 
    }
  }

  void off() { 
    if (LED_PIN != NO_PIN && state) { 
      digitalWrite(LED_PIN, HIGH); 
      state = false;
      Serial.printf("Relay %d OFF!\n", LED_PIN); 
    }
  }
};