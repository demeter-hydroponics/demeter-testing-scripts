#include <Arduino.h>
#include <Wire.h>
#include <LTR303.h>

// Define pins
#define PERIOD 100          // Sampling period in milliseconds
#define SDA_PIN 27          // SDA pin for I2C
#define SCL_PIN 26          // SCL pin for I2C

static LTR303 light;

unsigned char gain;     // Gain setting, values = 0-7 
unsigned char integrationTime;  // Integration ("shutter") time in milliseconds
unsigned char measurementRate;  // Interval between DATA_REGISTERS update

void printError(byte error);

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        // Wait for Serial to initialize
    }

    // print a serial init message
    Serial.println("Serial Initialized!");
    delay(100);
// You can pass nothing to light.begin() for the default I2C address (0x29)
  Wire.begin(SDA_PIN, SCL_PIN);
  light.begin();

  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)

  unsigned char ID;
  
  if (light.getPartID(ID)) {
    Serial.print("Got Sensor Part ID: 0X");
    Serial.print(ID,HEX);
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else {
    byte error = light.getError();
    printError(error);
  }

  // The light sensor has a default integration time of 100ms,
  // and a default gain of low (1X).
  
  // If you would like to change either of these, you can
  // do so using the setControl() and setMeasurementRate() command.
  
  // Gain can take any value from 0-7, except 4 & 5
  // If gain = 0, device is set to 1X gain (default)
  // If gain = 1, device is set to 2X gain
  // If gain = 2, device is set to 4X gain
  // If gain = 3, device is set to 8X gain
  // If gain = 4, invalid
  // If gain = 5, invalid
  // If gain = 6, device is set to 48X gain
  // If gain = 7, device is set to 96X gain
  gain = 0;
  Serial.println("Setting Gain...");
  light.setControl(gain, false, false);

  // If integrationTime = 0, integrationTime will be 100ms (default)
  // If integrationTime = 1, integrationTime will be 50ms
  // If integrationTime = 2, integrationTime will be 200ms
  // If integrationTime = 3, integrationTime will be 400ms
  // If integrationTime = 4, integrationTime will be 150ms
  // If integrationTime = 5, integrationTime will be 250ms
  // If integrationTime = 6, integrationTime will be 300ms
  // If integrationTime = 7, integrationTime will be 350ms

  unsigned char time = 1;

  // If integrationTime = 0, integrationTime will be 100ms (default)
  // If integrationTime = 1, integrationTime will be 50ms
  // If integrationTime = 2, integrationTime will be 200ms
  // If integrationTime = 3, integrationTime will be 400ms
  // If integrationTime = 4, integrationTime will be 150ms
  // If integrationTime = 5, integrationTime will be 250ms
  // If integrationTime = 6, integrationTime will be 300ms
  // If integrationTime = 7, integrationTime will be 350ms
  
  Serial.println("Set timing...");
  light.setMeasurementRate(time,3);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
  
  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.
}

void loop() {
  // Wait between measurements before retrieving the result
  // You can also configure the sensor to issue an interrupt 
  // when measurements are complete)
  
  // This sketch uses the LTR303's built-in integration timer.
  
  int ms = 1000;
  
  delay(ms);
  
  
  // Once integration is complete, we'll retrieve the data.
  
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.
  
  // Retrieve the data from the device:

  unsigned int data0, data1;
  
  if (light.getData(data0,data1)) {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.println(data0);
    Serial.print("data1: ");
    Serial.println(data1);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
  
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:

    good = light.getLux(gain,integrationTime,data0,data1,lux);
    
    // Print out the results:
	
    Serial.print(" lux: ");
    Serial.println(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }
}

void printError(byte error) {
  // If there's an I2C error, this function will
  // print out an explanation.

  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error) {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}