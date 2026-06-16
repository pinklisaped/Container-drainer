#define SETTINGS_H

#include <Arduino.h>
#include <IPAddress.h>

#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#define DEVICE_SERIAL "00001"
#define DEVICE_NAME "ESP Drainer " DEVICE_SERIAL

#pragma once

#define PUMP_PIN D1
#define SENSOR_POWER_PIN D2
constexpr const uint16_t SLEEP_DURATION_SEC = 60;
constexpr const uint16_t MIN_SPILL_LEVEL = 300;
constexpr const uint16_t MAX_SPILL_SECONDS = 600;

// SETTINGS_H