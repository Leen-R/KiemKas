#pragma once

#include "DS18B20.h"

#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);
DS18B20 sensor(&oneWire);

class Tmp_DS18B12{
public:

  float tmp;

  void setup(void)
  {
    sensor.begin();
    sensor.setResolution(11);

    if(sensor.isConnected()) {
      sensor.requestTemperatures();  
    }
  }

  void update()
  {
    if(!sensor.isConnected()) {
      Serial.println("DS18B20 not connected!");
      return;
    }
    
    

    //  wait until sensor is ready
    if (sensor.isConversionComplete())  
    {

    tmp = sensor.getTempC();
    Serial.print("Temp: ");
    Serial.println(tmp);
    }

    sensor.requestTemperatures();

  }

};