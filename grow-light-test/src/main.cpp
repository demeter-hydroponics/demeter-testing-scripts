#include <Arduino.h>
#include <Wire.h>

#define LED_PWM_PIN 2

#define LED_ONTIME_SECONDS (60 * 60 * 12) // 12 hours
#define TIME_PER_DAY (60 * 60 * 24) // 24 hours

#define LED_BRIGHTNESS (255) // 0-255

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        // Wait for Serial to initialize
    }

    // Initialize LED PWM pin
    pinMode(LED_PWM_PIN, OUTPUT);
}

void loop() {
    const uint64_t timeMs = millis();

    const uint64_t timeInDay = timeMs % TIME_PER_DAY;
    const bool isDay = timeInDay < LED_ONTIME_SECONDS;

    const uint8_t brightness = isDay ? LED_BRIGHTNESS : 0;

    analogWrite(LED_PWM_PIN, brightness);
}
