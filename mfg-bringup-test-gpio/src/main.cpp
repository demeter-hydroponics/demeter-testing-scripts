#include <Arduino.h>


int readIntFromSerial() {
  
  while (Serial.available() == 0) {
    
  }
  
  int value = Serial.parseInt();
  // Clear out any extra characters (e.g. newline)
  while (Serial.available() > 0) {
    Serial.read();
  }
  return value;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {

  }
  Serial.println("=== Arduino Pin CLI ===");
  Serial.println("This program allows you to configure a pin and read/write its value.");
  Serial.println("Steps:");
  Serial.println("  1) Enter pin number (e.g. 13, 2, A0 -> which is 14 on Uno, etc.)");
  Serial.println("  2) Select mode: 0=Digital Input, 1=Digital Output, 2=Analog Input");
  Serial.println("  3) If Input or Analog, it will continuously print the value.");
  Serial.println("     If Output, you will be asked to enter a 0 or 1.");
  Serial.println("Enter a new pin at any time to change configuration.");
  Serial.println("========================================");
}

void loop() {
  static int currentPin = -1;       // The pin currently in use
  static int currentMode = -1;      // The mode currently set: 0=Input, 1=Output, 2=Analog
  static bool isConfigured = false; // Whether we've set up a pin/mode yet

  // If we haven’t configured a pin yet, or if user typed something, handle it
  if (!isConfigured || Serial.available() > 0) {
    // Flush any existing input
    while (Serial.available() > 0) {
      Serial.read();
    }
    Serial.println("");
    Serial.print("Enter pin number: ");
    currentPin = readIntFromSerial();

    Serial.print("Select mode. (0 = Input, 1 = Output, 2 = Analog): ");
    currentMode = readIntFromSerial();

    // Configure the pin accordingly
    switch (currentMode) {
      case 0: 
        pinMode(currentPin, INPUT);
        Serial.print("Pin ");
        Serial.print(currentPin);
        Serial.println(" configured as DIGITAL INPUT.");
        break;

      case 1:
        pinMode(currentPin, OUTPUT);
        Serial.print("Pin ");
        Serial.print(currentPin);
        Serial.println(" configured as DIGITAL OUTPUT.");
        break;

      case 2: 
        pinMode(currentPin, INPUT);
        Serial.print("Pin ");
        Serial.print(currentPin);
        Serial.println(" configured as ANALOG INPUT.");
        break;

      default:
        Serial.println("Invalid mode selected. Please try again.");
        return; // Go back to loop start, user will be asked again
    }

    isConfigured = true;

    // If the mode is OUTPUT, we ask for a value to write immediately
    if (currentMode == 1) {
      Serial.print("Enter value (0 or 1) to digitalWrite: ");
      int outValue = readIntFromSerial();
      digitalWrite(currentPin, (outValue == 0) ? LOW : HIGH);
      Serial.print("Wrote ");
      Serial.print(outValue);
      Serial.print(" to pin ");
      Serial.println(currentPin);
    }
    // Write instructions on how to exit
    Serial.println("Enter a new character at any time to change configuration.");
  }

  // If the pin is an input (digital or analog), we continuously read it
  if (isConfigured && (currentMode == 0 || currentMode == 2)) {
    if (currentMode == 0) {
      // Digital Input
      int val = digitalRead(currentPin);
      Serial.print("Digital pin ");
      Serial.print(currentPin);
      Serial.print(" reads: ");
      Serial.println(val);
    } else {
      // Analog Input
      int val = analogRead(currentPin); 
      Serial.print("Analog pin ");
      Serial.print(currentPin);
      Serial.print(" reads: ");
      Serial.println(val);
    }

    delay(500);
  }

}
