#pragma once
#include <Arduino.h>
#include <BlockNot.h>

int getThirstLevel();

#include "WiFiSetup.h"
#include "Broker.h"
#include "Sensors/Sensors.h"
#include "Actuators/Actuator_headers.h"

void regulate();
void pubSensors();
void callback(String topic, byte* message, unsigned int length);
int schedule(String timeStr);

void check_schedule();