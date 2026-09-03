#pragma once

Relay  heat;
Relay  heaterfan;

class Heater{

public:
  Heater(int heater_pin, int fan_pin = -1){
    heat.setup(heater_pin);
    heaterfan.setup(fan_pin);
  }

  void on()  { heat.on (); heaterfan.on (); }
  void off() { heat.off(); heaterfan.off(); }
};