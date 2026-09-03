#pragma once

#include <PubSubClient.h>
#include <BlockNot.h> // Toegevoegd voor de timer

WiFiClient    espClient;
PubSubClient  client(espClient);

extern void callback(String topic, byte* message, unsigned int length);

class Broker
{
private:
  const char* MQTT_username = "Kasclient"; 
  const char* MQTT_password = "Halt2001"; 
  const char* MQTT_server   = "192.168.1.150";
  
  // Timer: Probeer elke 10 seconden opnieuw te verbinden met MQTT
  BlockNot mqttReconnectTimer = BlockNot(10, SECONDS); 

  void subscriptions(){
    client.subscribe("kasklein/#");
  }

public: 
  void begin(){
    client.setCallback(callback); 
    client.setServer(MQTT_server, 1883);
    // Let op: we blokkeren hier niet meer bij het opstarten!
  }

  void update(){ 
    if (client.connected()) {
      client.loop(); 
    }
  }

void handleConnection() {
    // FIX: Actively force a WiFi reconnect if the router drops the DHCP lease
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi dropped. Forcing reconnect...");
      WiFi.disconnect();
      WiFi.reconnect();
      return; // Stop here and wait for WiFi to recover
    }

    // Alleen proberen als WiFi werkt, en MQTT niet verbonden is
    if (!client.connected()) {
      // Probeer het maximaal 1x per 10 seconden
      if (mqttReconnectTimer.TRIGGERED) {
        Serial.print("Attempting MQTT connection...");
        
        if (client.connect("ESP32KasKleinClient", MQTT_username, MQTT_password)) {
          Serial.println("connected");
          subscriptions();
        } else {
          Serial.print("failed, rc=");
          Serial.println(client.state()); // Print foutcode en ga direct door
        }
      }
    }
  }

void publish(const char* topic, String message) {
    // Gebruik een statische buffer in plaats van String concatenatie
    if (client.connected()) {
      char fullTopic[64];
      snprintf(fullTopic, sizeof(fullTopic), "kasklein/%s", topic);
      client.publish(fullTopic, message.c_str());
    }
  }

};