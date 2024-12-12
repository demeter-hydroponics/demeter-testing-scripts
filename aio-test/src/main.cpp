#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Define pins
#define PERIOD 100          // Sampling period in milliseconds
#define ANALOG_PIN 33       // Analog input pin
#define SDA_PIN 5           // SDA pin for I2C
#define SCL_PIN 18          // SCL pin for I2C

// Constants for analog voltage calculation
constexpr float VOLTAGE_MAX = 3.3;
constexpr float ADC_MAX = 4095;
constexpr float BITS_TO_VOLTS = VOLTAGE_MAX / ADC_MAX;

//#define USE_TOF_SENSE

#ifdef USE_TOF_SENSE
// VL53L0X sensor instance
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#endif

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        // Wait for Serial to initialize
    }

    #ifdef USE_TOF_SENSE
    // Initialize I2C
    Wire.begin(SDA_PIN, SCL_PIN);
    

    // Initialize VL53L0X sensor
    if (!lox.begin()) {
        Serial.println("Failed to initialize VL53L0X. Check your connections!");
    }
    Serial.println("VL53L0X Initialized!");
    #endif
}

void loop() {
    // Read analog input
    uint16_t rawValue = analogRead(ANALOG_PIN);
    float voltage = rawValue * BITS_TO_VOLTS;

    // Print analog voltage
    Serial.print("Analog Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    #ifdef USE_TOF_SENSE
    // Read distance from VL53L0X sensor
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // Perform a ranging test
    if (measure.RangeStatus != 4) {   // If range is valid
        int distance = measure.RangeMilliMeter;
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
    } else {
        Serial.println("Distance: Out of range");
    }
    #endif

    // Wait for the specified period
    delay(PERIOD);
}
