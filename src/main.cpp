#include <Arduino.h>
#include <settings.h>

#define PUMP_PIN D1
#define SENSOR_POWER_PIN D2

volatile uint16_t forceSpillDuration = 0;

uint16_t drain(uint16_t minStopLevel = MIN_SPILL_LEVEL / 2, uint16_t maxSpillSeconds = MAX_SPILL_SECONDS);
void drainForce(uint16_t spillSeconds);
void sleep(uint32_t interval = SLEEP_DURATION_SEC);
void blinkLED1s();

void setup()
{
#ifdef DEBUG_ENABLE
    Serial.begin(115200); // COM init
#endif

    pinMode(PUMP_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SENSOR_POWER_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("Device started");
}

void loop()
{
    digitalWrite(SENSOR_POWER_PIN, HIGH);
    delay(500);
    uint16_t waterLevel = analogRead(A0);
    DEBUG("Water level is " + String(waterLevel));
    if (waterLevel >= MIN_SPILL_LEVEL)
    {
        uint16_t spillSeconds = drain(MIN_SPILL_LEVEL / 1.5, MAX_SPILL_SECONDS);
        digitalWrite(SENSOR_POWER_PIN, LOW);
        drainForce(spillSeconds / 2);
    }
    digitalWrite(SENSOR_POWER_PIN, LOW);
    delay(500);
    sleep(SLEEP_DURATION_SEC);
}

uint16_t drain(uint16_t minStopLevel, uint16_t maxSpillSeconds)
{
    uint16_t waterLevel = 0;
    uint16_t spillSeconds = 0;
    digitalWrite(PUMP_PIN, HIGH);
    do
    {
        blinkLED1s();
        spillSeconds += 1;
        waterLevel = analogRead(A0);
        DEBUG("Drain " + String(spillSeconds) + " sec, water level is " + String(waterLevel));
    } while (waterLevel >= minStopLevel && spillSeconds < maxSpillSeconds);
    digitalWrite(PUMP_PIN, LOW);
    return spillSeconds;
}

void drainForce(uint16_t spillSeconds)
{
    digitalWrite(PUMP_PIN, HIGH);
    for (; spillSeconds > 0; spillSeconds--)
        blinkLED1s();
    digitalWrite(PUMP_PIN, LOW);
}

void blinkLED1s()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(750);
}

void sleep(uint32_t sleepDurationSec)
{
    DEBUG("Go sleep " + String(sleepDurationSec) + " sec");
    ESP.deepSleep(sleepDurationSec * 1000000);
}
