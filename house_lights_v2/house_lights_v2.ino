#include "ShiftRegister74HC595.h"
#include "button_reader.h"
#include "light_manager.h"

#define BYTES_VAL_T unsigned long
#define LED_BUILTIN 2
#define DATA_WIDTH 32

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<4> sr(26, 33, 25);

void flashLed() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  initButtonReader();

  // Turn off all relays on restart
  sr.setAllLow();
  oldPinValues = readShiftRegisterValues();
}

void toggle(int pin) {
        uint8_t currentState = sr.get(pin);
        sr.set(pin, currentState ^ HIGH);
        Serial.print("  Toggling-");
        Serial.print(pin);
        Serial.print(": oldState:");
        Serial.print(currentState);
        Serial.print(": newState:");
        Serial.print(currentState ^ HIGH);
    }

void loop() {
  pinValues = readShiftRegisterValues();
  
  // If there was a chage in button state, display which ones changed
  if (pinValues != oldPinValues) {
    if (isButtonClicked(pinValues)) {
      for (int i = 0; i < DATA_WIDTH; i++) {
        if ((pinValues >> i) & 1) {
          toggle(i);
        }
      }
      flashLed();
    }

    oldPinValues = pinValues;
  }
}
