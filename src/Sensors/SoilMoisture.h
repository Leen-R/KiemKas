#pragma once
#include "ADS1X15.h"

ADS1115 ADS(0x48);

class SoilMoisture{
public:

  // Alle vier de variabelen bevatten nu het eindpercentage (0-100)
  int v0;  
  int v1;  
  int v2;  
  int v3;
  
  // Jouw kalibratiewaarden (deze gelden nu voor alle 4 de sensoren)
  int wetValue = 5180;  
  int dryValue = 13310; 

  void setup() 
  {
    Serial.println(__FILE__);
    Serial.print("ADS1X15_LIB_VERSION: ");
    Serial.println(ADS1X15_LIB_VERSION);

    Wire.begin();
    ADS.begin();
  }

 
  // Deze functie pakt een ruwe waarde en zet hem veilig om naar 0-100%
  int calculatePercentage(int16_t rawValue) {
    int percentage = map(rawValue, dryValue, wetValue, 0, 100);
    
    // Begrens de waarde
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;
    
    return percentage;
  }
  // --------------------------

  void update() 
  {
    ADS.setGain(0);

    // 1. Lees de ruwe data uit van alle vier de kanalen
    int16_t raw_0 = ADS.readADC(0);
    int16_t raw_1 = ADS.readADC(1);
    int16_t raw_2 = ADS.readADC(2);
    int16_t raw_3 = ADS.readADC(3);

    // 2. Stuur de ruwe data door onze hulpfunctie om het percentage te berekenen
    v0 = calculatePercentage(raw_0);
    v1 = calculatePercentage(raw_1);
    v2 = calculatePercentage(raw_2);
    v3 = calculatePercentage(raw_3);

    // Print de resultaten overzichtelijk in de Seriële Monitor
    Serial.print("\tSensor 0 (RAW): "); Serial.print(raw_0); Serial.print("\tVocht: "); Serial.print(v0); Serial.println("%");
    Serial.print("\tSensor 1 (RAW): "); Serial.print(raw_1); Serial.print("\tVocht: "); Serial.print(v1); Serial.println("%");
    Serial.print("\tSensor 2 (RAW): "); Serial.print(raw_2); Serial.print("\tVocht: "); Serial.print(v2); Serial.println("%");
    Serial.print("\tSensor 3 (RAW): "); Serial.print(raw_3); Serial.print("\tVocht: "); Serial.print(v3); Serial.println("%");
    Serial.println();
  }
};