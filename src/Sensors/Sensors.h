#pragma once

#include "Tmp_DS18B20.h"
#include "SoilMoisture.h"

#include "Humidity.h"

class Sensors {
public:
  float tmp_air;
  float tmp_lamp;
  float humidity;
  //float vpd;
  
 

  int16_t soil_0;  
  int16_t soil_1;  
  int16_t soil_2;  
  int16_t soil_3;

  

  void setup() {
    
    tmp.setup();
    sht.setup();
  
  }

  void refresh(){
    
    tmp.update();
    sms.update();
    sht.update();

    //co2.loop();
  	
    tmp_lamp	   = tmp.tmp;
    tmp_air      = sht.aTemperature;
    humidity     = sht.aHumidity;
    
  //vpd          = humi.vpd;


    soil_0       = sms.v0;
    soil_1       = sms.v1;
    soil_2       = sms.v2;
    soil_3       = sms.v3;

  }

private:
  Tmp_DS18B12  tmp;
  SoilMoisture sms;
  SHT40        sht;
  

  
};
