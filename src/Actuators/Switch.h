#pragma once

class Switch {
  int SWITCH_PIN;
  bool state = false; // Tracks the current state to prevent spam

public:
  Switch(int pin = NO_PIN) { setup(pin); }

  void setup(int pin) {
    SWITCH_PIN = pin;
    if (SWITCH_PIN != NO_PIN) { 
      pinMode(SWITCH_PIN, OUTPUT); 
      digitalWrite(SWITCH_PIN, LOW); // Force hardware to match our 'false' state
    }
  }

  void on() { 
    if (SWITCH_PIN != NO_PIN && !state) { 
      digitalWrite(SWITCH_PIN, HIGH); 
      state = true;
      Serial.printf("Switch %d ON!\n", SWITCH_PIN); 
    }
  }

  void off() { 
    if (SWITCH_PIN != NO_PIN && state) { 
      digitalWrite(SWITCH_PIN, LOW);  
      state = false;
      Serial.printf("Switch %d OFF!\n", SWITCH_PIN); 
    }
  }
};