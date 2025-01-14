#include <Arduino.h>
#include <LTR303.h>

// Create an instance of the LTR303 sensor
LTR303 ltr303;

#define SDA_PIN 27
#define SCL_PIN 26

void setup() {
    Serial.begin(9600);
    Serial.println(__FILE__);
    Serial.printf("LTR303 Library Version: %s\r\n\r\n", LTR303_LIB_VERSION);

    Wire1.begin(SDA_PIN, SCL_PIN);  //Uses default pins
    // Wire1.begin(/*Write SDA_PIN here*/, /*Write SCL_PIN here*/); //custom i2c pins
    // Wire1.begin(/*Write SDA_PIN here*/, /*Write SCL_PIN here*/, 500000);  // tested to be 380khz irl on the esp32 (<400khz per data sheet)

    // Initialize the LTR303 sensor with default settings (1X gain, 100ms exposure)
    uint8_t error = ltr303.begin(GAIN_1X, EXPOSURE_100ms, true, Wire1);
    if (error != 0) {
        Serial.printf("Error initializing LTR303: %s\r\n", ltr303.getErrorText(error));
        return;
    }

    // Start periodic measurement mode
    ltr303.startPeriodicMeasurement();
}

void loop() {
    while (!ltr303.newDataAvailable()) {
        delay(50);  // wait a short time before checking again
    }

    // Get the raw data from the LTR303 sensor
    uint16_t visibleAndIRraw = 0;
    uint16_t IRraw = 0;
    uint8_t error = ltr303.getData(visibleAndIRraw, IRraw);
    if (error != 0) {
        Serial.printf("Error reading LTR303 data: %s\r\n", ltr303.getErrorText(error));
    } else {
        //Serial.printf("Visible + IR = %u,   IR = %u\r\n", visibleAndIRraw, IRraw);
    }


    // Get an approximate lux value from the LTR303 sensor
    double lux = 0.0;
    while (ltr303.getApproximateLux(lux) == false) {
        delay(50);  // wait a short time before checking again
    }
    Serial.printf("Light Level = %8.4flux\r\n", lux);
}