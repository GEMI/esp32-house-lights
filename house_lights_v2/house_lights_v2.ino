#include "button_reader.h"
#include "light_manager.h"

#define BYTES_VAL_T unsigned long
#define LED_BUILTIN 2
#define DATA_WIDTH 32
const int STATE_OFF = 0;
const int STATE_ON = 1;

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

int outputs[DATA_WIDTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
int state[DATA_WIDTH] = { STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF, STATE_OFF };

// Shift register for outputs
int dataPin = 26;   // blue
int latchPin = 25;  // green
int clockPin = 33;  // yellow/white

void flashLed() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
}

void writeRegister74HC595State() {
  digitalWrite(latchPin, 0);

  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);

  for (int i = DATA_WIDTH - 1; i >= 0; i--) {
    digitalWrite(clockPin, 0);

    int pinState = 0;
    switch (state[outputs[i]]) {
      case STATE_ON:
        pinState = 1;
        break;
      case STATE_OFF:
        pinState = 0;
        break;
    }

    //Sets the pin to HIGH or LOW depending on pin state
    digitalWrite(dataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(clockPin, 1);

    //zero the data pin after shift to prevent bleed through
    digitalWrite(dataPin, 0);
  }

  //stop shifting
  digitalWrite(clockPin, 0);
  digitalWrite(latchPin, 1);
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  initButtonReader();

  oldPinValues = readShiftRegisterValues();

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void toggle(int pin) {
  int currState = state[pin];
  Serial.print("  Toggling light nr. ");
  Serial.print(pin);
  Serial.print(" , oldState-> ");
  Serial.print(currState);
  Serial.print("\r\n");
}

void loop() {
  pinValues = readShiftRegisterValues();

  // If there was a chage in button state, display which ones changed
  if (pinValues != oldPinValues) {
    if (isButtonClicked(pinValues)) {
      for (int i = 0; i < DATA_WIDTH; i++) {
        if ((pinValues >> i) & 1) {
          toggle(i);
          int currState = state[i];
          state[i] = (currState == STATE_ON) ? STATE_OFF : STATE_ON;
          writeRegister74HC595State();
        }
      }
      flashLed();
    }

    oldPinValues = pinValues;
  }
}
