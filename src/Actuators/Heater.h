#pragma once
#include "Relay.h"

class Heater {
private:
  Relay heat;
  Relay heaterfan;

public:
  Heater(int heater_pin, int fan_pin = NO_PIN){
    heat.setup(heater_pin);
    heaterfan.setup(fan_pin);
  }

  void on()  { heat.on (); heaterfan.on (); }
  void off() { heat.off(); heaterfan.off(); }
};