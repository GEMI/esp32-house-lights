#include "ShiftRegister74HC595.h"
#include "button_reader.h"
#include "light_manager.h"

#define BYTES_VAL_T unsigned long
#define LED_BUILTIN 2
#define SHIFT_REGISTER_COUNT 3
#define DATA_WIDTH SHIFT_REGISTER_COUNT * 8

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

// Turn OFF all relays on startup
// The 16 relay board is trigered by LOW pins
// So setting 0b00000000 would set all relays to turn ON
byte RELAY_STATE[1] = {
  0b00000000
};


// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<1> sr(21, 26, 33);

int datArray[8];
int datArray2[8];

void flashLed() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
}

void toggleHighPins(BYTES_VAL_T pins, byte* state) {
  for (int i = 0; i < DATA_WIDTH; i++) {
    if ((pins >> i) & 1) {
      // If pin is high, toggle change of the corresponding bit in the relay state
      int groupNumber = i / 8;
      int bitNumber = i % 8;
      toggleBit(state, groupNumber, bitNumber);
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  initButtonReader();

  // Turn off all relays on restart
  sr.setAllHigh();

  pinValues = readShiftRegisterValues();
  displayPinValues(pinValues);
  oldPinValues = pinValues;
}

void loop() {
  pinValues = readShiftRegisterValues();


  // If there was a chage in button state, display which ones changed
  if (pinValues != oldPinValues) {
    if (isButtonClicked(pinValues)) {

      for (int numberToDisplay = 0; numberToDisplay < 8; numberToDisplay++) {
        Serial.print(" LOW->");
        Serial.print(numberToDisplay);
        sr.set(numberToDisplay, LOW);

        delay(200);

        Serial.print(" High->");
        Serial.print(numberToDisplay);
        sr.set(numberToDisplay, HIGH);
      }


      // Button was clicked, update RELAY_STATE and write this new state to relays
      toggleHighPins(pinValues, RELAY_STATE);
      // writeDataToRelays(RELAY_STATE_OFF);
  
      // This display is for debugging purposes only
      displayPinValues(pinValues);
      flashLed();
    }

    oldPinValues = pinValues;
  }
}
