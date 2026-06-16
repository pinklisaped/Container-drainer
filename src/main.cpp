#include <Arduino.h>
#include <settings.h>

volatile uint16_t forceSpillDuration = 0;

uint16_t drain(uint16_t minStopLevel = MIN_SPILL_LEVEL / 2, uint16_t maxSpillSeconds = MAX_SPILL_SECONDS);
void drainForce(uint16_t spillSeconds);
void sleep(uint32_t interval = SLEEP_DURATION_SEC);
void blinkLED(unsigned long on = 750, unsigned long off = 250);

void setup()
{
#ifdef DEBUG_ENABLE
    Serial.begin(9600); // COM init
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
        uint16_t spillSeconds = drain(MIN_SPILL_LEVEL / 1.5, MAX_SPILL_SECONDS / 2);
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
    uint32_t startTime = millis();
    uint16_t spillSeconds = 0;

    digitalWrite(PUMP_PIN, HIGH);

    do
    {
        blinkLED();
        spillSeconds = (millis() - startTime) / 1000;
        waterLevel = analogRead(A0);
        DEBUG("Drain " + String(spillSeconds) + " sec, water level is " + String(waterLevel));

        yield();

    } while (waterLevel >= minStopLevel && spillSeconds < maxSpillSeconds);

    digitalWrite(PUMP_PIN, LOW);
    return spillSeconds;
}

void drainForce(uint16_t spillSeconds)
{
    long remainingMs = (long)spillSeconds * 1000;
    digitalWrite(PUMP_PIN, HIGH);
    while (remainingMs > 0)
    {
        blinkLED(50, 150);
        remainingMs -= 400;

        yield();
    }

    digitalWrite(PUMP_PIN, LOW);
}

void blinkLED(unsigned long on, unsigned long off)
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(off);
    digitalWrite(LED_BUILTIN, LOW);
    delay(on);
}

void sleep(uint32_t sleepDurationSec)
{
    DEBUG("Go sleep " + String(sleepDurationSec) + " sec");
    ESP.deepSleep(sleepDurationSec * 1000000);
}
