#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;


Pomp   pomp;
Fan    fan;
Heater heater     (15, -1);
Switch humidifier (5);


float optimal[10] = { 
//val   i actu    factor      
  28, //0 fan     air tmp     lower
  32, //1 fan     air tmp     upper
  80, //2 fan     humidity    lower
  95, //3 fan     humidity    upper
  40, //4 pomp    soil moisture
  60, //5 humi    humidity
  18, //6 heater  temperature 
  80, //7 
  80, //8
  80  //9
  };

// Standaardwaarden, bijvoorbeeld 08:00 (480 min) tot 20:00 (1200 min)
int timeOn  = 8 * 60;  
int timeOff = 20 * 60;

bool scheduleMode;

BlockNot  update        (5, SECONDS);

void printOptimalValues() {
  Serial.println("Current optimal values:");

  for (int i = 0; i < 10; i++) {
    Serial.print("optimal[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(optimal[i]);
  }
}


void setup() {
  Serial.begin(115200);
  wifi.setup();
  broker.begin(); 
  sensors.setup();
  broker.publish("status/kasklein", "online");
}

void loop() {
  wifi.handleTime();
  broker.handleConnection(); 
  broker.update();
  fan.update();
  pomp.update();

  if(update.TRIGGERED){
    sensors.refresh();
    regulate();
    check_schedule();
    //printOptimalValues();

    if (client.connected()) {
      pubSensors();
    }

  }
}

void regulate(){
 
  sensors.humidity < optimal[5] ? humidifier.off() : humidifier.on();
  sensors.tmp_air  < optimal[6] ? heater.on()      : heater.off();

  sensors.tmp_air  < optimal[6] ? heaterfan.on()   : heaterfan.off();
}

void check_schedule(){
  int now = wifi.nowTimeMin();

  if (now < 0) return;

  bool shouldBeOn = false;

  if (timeOn < timeOff) {
    // Normaal schema overdag (bijv. 08:00 tot 20:00)
    if (now >= timeOn && now < timeOff) {
      shouldBeOn = true;
    }
  } else if (timeOn > timeOff) {
    // Nachtschema dat over middernacht gaat (bijv. 22:00 tot 06:00)
    if (now >= timeOn || now < timeOff) {
      shouldBeOn = true;
    }
  }



}

int schedule(String timeStr) {
  int h, m, s, timeMin;
  sscanf(timeStr.c_str(), "%d:%d:%d", &h, &m, &s);

  timeMin = h*60 + m;
  return timeMin;
}

void pubSensors(){
  broker.publish("tmp/air"  , String(sensors.tmp_air ));
  broker.publish("tmp/soil" , String(sensors.tmp_lamp));
  broker.publish("vocht"    , String(sensors.humidity));
  //broker.publish("lux"      , String(sensors.lux     ));
//broker.publish("vpd"      , String(sensors.vpd     ));
  broker.publish("soil"     , String(sensors.soil_1  ));
//broker.publish("CO2"      , String(sensors.eCO2    ));

}

void callback(String topic, byte* message, unsigned int length) {
  topic = topic.substring(9);
  
  // FIX: Allocate memory once to prevent heap fragmentation
  char msgBuffer[length + 1];
  memcpy(msgBuffer, message, length);
  msgBuffer[length] = '\0';
  String msg = String(msgBuffer);

  if(topic == "schedule/on")   timeOn  = schedule(msg);
  if(topic == "schedule/off")  timeOff = schedule(msg);

  if(topic == "pomp"){
    if(msg == "on")  pomp.supplyWater();
    else if(msg == "off") waterpump.off();
    else flowTimer.setDuration(msg.toInt());  
  }
  if(topic == "humi"){
    if(msg == "on")  { humidifier.on();   }
    if(msg == "off") { humidifier.off();  }
  }
  if(topic == "optimal"){
    int i, val;
    sscanf(msg.c_str(), "%d %d", &i, &val); 
    optimal[i] = val;
    printOptimalValues();
  }
}

int getThirstLevel(){
  int soil = sensors.soil_1;
  return (optimal[4] - soil);
}
