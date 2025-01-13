#include <Arduino.h>
#include <Wire.h>

// Define pins
#define PERIOD 100          // Sampling period in milliseconds
#define SDA_PIN 21          // SDA pin for I2C
#define SCL_PIN 22          // SCL pin for I2C

// Uncomment the driver you want to use
//#define USE_ADAFRUIT_TOF
#define USE_POLOLU_TOF

#ifdef USE_ADAFRUIT_TOF
#include <Adafruit_VL53L0X.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#endif

#ifdef USE_POLOLU_TOF
#include <VL53L0X.h>
VL53L0X sensor;
#endif

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        // Wait for Serial to initialize
    }

    // print a serial init message
    Serial.println("Serial Initialized!");
    delay(3);

    #ifdef USE_ADAFRUIT_TOF
    // Initialize I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize Adafruit VL53L0X sensor
    if (!lox.begin(VL53L0X_I2C_ADDR, true)) {
        Serial.println("Failed to initialize Adafruit VL53L0X. Check your connections!");
        while (1); // Halt if initialization fails
    }
    Serial.println("Adafruit VL53L0X Initialized!");
    #endif

    #ifdef USE_POLOLU_TOF
    // Initialize I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize Pololu VL53L0X sensor
    if (!sensor.init()) {
        Serial.println("Failed to initialize Pololu VL53L0X. Check your connections!");
        while (1); // Halt if initialization fails
    }
    sensor.setTimeout(500);
    Serial.println("Pololu VL53L0X Initialized!");

    // Optional: Set measurement timing budget (minimum 20 ms)
    sensor.setMeasurementTimingBudget(20000); // 20 ms
    #endif
}

void loop() {
    #ifdef USE_ADAFRUIT_TOF
    // Read distance from Adafruit VL53L0X sensor
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // Perform a ranging test
    if (measure.RangeStatus != 4) {   // If range is valid
        Serial.print("Distance: ");
        Serial.print(measure.RangeMilliMeter);
        Serial.println(" mm");
    } else {
        Serial.println("Distance: Out of range");
    }
    #endif

    #ifdef USE_POLOLU_TOF
    // Read distance from Pololu VL53L0X sensor
    uint16_t distance = sensor.readRangeSingleMillimeters();

    if (sensor.timeoutOccurred()) {
        Serial.println("Sensor timeout!");
    } else {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
    }
    #endif

    // Wait for the specified period
    delay(PERIOD);
}
