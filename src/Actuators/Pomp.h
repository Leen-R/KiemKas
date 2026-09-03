#pragma once

BlockNot flowTimer (2000);
BlockNot soilCheck (1000* 60* 3); //(8*3600 , SECONDS);
Relay    waterpump (18);


class Pomp{
public:
  void update(){
    blockWater();
    if(soilCheck.TRIGGERED)
      CheckSoilMoisture();
  }

  void CheckSoilMoisture(){
  int thirst = getThirstLevel();
  if(thirst > 20){ 
    set_milliLiters(thirst*10);
    supplyWater();
  }
}

void set_milliLiters(int ml){
  flowTimer.setDuration(floor(ml/0.0158)); //measured const 0.0158 ml/ms = 15.8 ml/s
}

void supplyWater(){
  waterpump.on();
  flowTimer.RESET;
} 

void blockWater(){
  if(flowTimer.FIRST_TRIGGER)
    waterpump.off();
  }
};
