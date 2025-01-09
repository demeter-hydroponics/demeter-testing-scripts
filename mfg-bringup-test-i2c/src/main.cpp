#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 18
#define SCL_PIN 19
#define I2C_ADDR 0x50

void setup() {
  Serial.begin(9600);
  delay(100); // Allow Serial to stabilize

  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C Scanner Initialized.");
}

void loop() {
  // Perform I2C bus scan
  Serial.println("Scanning I2C bus...");
  for (int i = 0; i < 128; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at address: 0x");
      Serial.println(i, HEX);
    }
  }
  delay(1000); // Pause before next scan

  // Read a byte from the specified I2C address
  Serial.print("Reading from address 0x");
  Serial.println(I2C_ADDR, HEX);
  Wire.requestFrom(I2C_ADDR, 1);
  if (Wire.available()) {
    int data = Wire.read();
    Serial.print("Data received: 0x");
    Serial.println(data, HEX);
  } else {
    Serial.println("No data received.");
  }

  delay(1000); // Pause before next loop iteration
}
