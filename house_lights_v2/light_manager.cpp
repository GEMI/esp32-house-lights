#include "light_manager.h"

// NOTE: you can power your arduino with a 5V line from the Relay board
// Arduino just needs a wire from Relay board 5V to arduin VCC pin
// This will allow to drop one Power supply
// Keep in mind that to control two relays we will need power supply of at least 5V - 2 AMP


// Set the number of shift registers 74HC595 you are using
// For each 16 in relay board we need 2 shift registers
#define SHIFT_REGISTER_COUNT 4

// Define which Arduino IO pins you are connecting to the relay group
int PIN_DATA = 26;           // Connects SER / Data pin / DS (Serial Data Input)
int PIN_CLOCK = 33;         // Connects SH_CP (Shift Register Clock Input)
int PIN_LATCH = 25;         // Connects to ST_CP (Storage Register Clock Input, Latch)


void toggleBit(byte* relayState, int index, int bitPosition) {
  relayState[index] ^= (1 << bitPosition);
}

void initLightManager(byte initialValues[]) {
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);

  writeDataToRelays(initialValues);
  delay(500);
}

void writeDataToRelays(byte data[]) {
  Serial.print("Writing data to relays!");
  for (int i = 0; i < SHIFT_REGISTER_COUNT * 8; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.print("\r\n");


  // digitalWrite(PIN_LATCH, LOW);
  // // Set all relays
  // for (int i = 0; i < SHIFT_REGISTER_COUNT * 8; i++) {
  //   shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, data[i]);
  // }
  // digitalWrite(PIN_LATCH, HIGH);
}
