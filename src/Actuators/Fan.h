#pragma once

BlockNot  fanUpdate  (3600, SECONDS);
BlockNot activeTime (60000);
Relay    fan_1 (17);


class Fan{
public:
  void update(){
    if(fanUpdate.TRIGGERED){
      fanOn();
    }

    FanOff();
  }


void fanOn(){
  fan_1.on();
  activeTime.RESET;
} 

void FanOff(){
  if(activeTime.FIRST_TRIGGER)
    fan_1.off();
  }
};
