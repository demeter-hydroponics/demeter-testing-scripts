#include <Arduino.h>


uint8_t nSLEEP = 23;
uint8_t solenoidPin = 27;
uint8_t pHPin = 35;
uint8_t tdsPin = 36; //analog input, TODO FILL IN
uint8_t delayTimeMSDefault = 50;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()>0){
    // Expect a string of form "0" to "1024", corresponding to the time to run the solenoid
    // in milliseconds

    uint16_t delayTimeMS = Serial.parseInt();

    if (delayTimeMS == 0){
      delayTimeMS = delayTimeMSDefault;
    }

    Serial.print("Actuating valve for ");
    Serial.print(delayTimeMS);
    Serial.println(" milliseconds");

    digitalWrite (solenoidPin,HIGH);
    delay(delayTimeMS);
    digitalWrite (solenoidPin, LOW);
  }
  uint8_t tdsValue = analogRead(tdsPin);

#ifdef PRINT_TDS_VALUE  
  constexpr float aioToVolts = 3.3 / 4095.0;

  Serial.print("TDS value: ");
  Serial.println(tdsValue * aioToVolts);

  delay(1000);
#else
 delay(10);
#endif
}
