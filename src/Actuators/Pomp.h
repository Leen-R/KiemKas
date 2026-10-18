#pragma once
#include "Relay.h"

class Pomp {
private:
  BlockNot flowTimer{2000};
  BlockNot soilCheck{1000 * 60 * 3}; 
  Relay waterpump{PIN_WATERPUMP};

public:
  void update() {
    blockWater();
    if(soilCheck.TRIGGERED)
      CheckSoilMoisture();
  }

  void CheckSoilMoisture() {
    int thirst = getThirstLevel();
    if(thirst > 20){ 
      set_milliLiters(thirst*10);
      supplyWater();
    }
  }

  void set_milliLiters(int ml) {
    flowTimer.setDuration(floor(ml/0.0158)); 
  }

  void setDuration(int ms){
    flowTimer.setDuration(ms);
  }

  void supplyWater() {
    waterpump.on();
    flowTimer.RESET;
  } 

  void turnOff(){
    waterpump.off();
  }

  void blockWater() {
    if(flowTimer.FIRST_TRIGGER)
      waterpump.off();
  }
};