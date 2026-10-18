#include "Files.h"

WiFiSetup wifi;
Broker    broker; 
Sensors   sensors;

Pomp   pomp;
Fan    fan;
Heater heater     (PIN_HEATER, PIN_HEATER_FAN);
Switch humidifier (PIN_HUMIDIFIER);

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

int timeOn  = 8 * 60;  
int timeOff = 20 * 60;

bool scheduleMode;

BlockNot update(5, SECONDS);

void printOptimalValues() {
  Serial.println("Current optimal values:");
  for (int i = 0; i < 10; i++) {
    Serial.printf("optimal[%d] = %.2f\n", i, optimal[i]);
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

    if (client.connected()) {
      pubSensors();
    }
  }
}

void regulate(){
  // FIXED: Turn ON when humidity is lower than optimal
  sensors.humidity < optimal[5] ? humidifier.on() : humidifier.off();
  
  // Heater turns on heat (and fan if configured) automatically
  sensors.tmp_lamp < optimal[6] ? heater.on() : heater.off();
}

void check_schedule(){
  int now = wifi.nowTimeMin();
  if (now < 0) return;

  bool shouldBeOn = false;

  if (timeOn < timeOff) {
    if (now >= timeOn && now < timeOff) {
      shouldBeOn = true;
    }
  } else if (timeOn > timeOff) {
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
  char msgBuffer[16]; 

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.tmp_air);
  broker.publish("tmp/air", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.tmp_lamp);
  broker.publish("tmp/soil", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%.2f", sensors.humidity);
  broker.publish("vocht", msgBuffer);

  snprintf(msgBuffer, sizeof(msgBuffer), "%d", sensors.soil_1);
  broker.publish("soil", msgBuffer);
}

void callback(String topic, byte* message, unsigned int length) {
  topic = topic.substring(9);
  
  char msgBuffer[length + 1];
  memcpy(msgBuffer, message, length);
  msgBuffer[length] = '\0';
  String msg = String(msgBuffer);

  if(topic == "schedule/on")   timeOn  = schedule(msg);
  if(topic == "schedule/off")  timeOff = schedule(msg);

  if(topic == "pomp"){
    if(msg == "on")  pomp.supplyWater();
    else if(msg == "off") pomp.turnOff();
    else pomp.setDuration(msg.toInt());  
  }
  
  if(topic == "humi"){
    if(msg == "on")  { humidifier.on();   }
    if(msg == "off") { humidifier.off();  }
  }
  
  if(topic == "optimal"){
    int i;
    float val;
    sscanf(msg.c_str(), "%d %f", &i, &val); 
    // FIXED: Protect against array out-of-bounds crash
    if (i >= 0 && i < 10) {
      optimal[i] = val;
      printOptimalValues();
    }
  }
}

int getThirstLevel(){
  int soil = sensors.soil_1;
  return (optimal[4] - soil);
}