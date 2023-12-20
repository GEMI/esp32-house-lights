#ifndef button_reader
#define button_reader

// https://playground.arduino.cc/Code/ShiftRegSN74HC165N/
#include <Arduino.h>

/* This function is essentially a "shift-in" routine reading the
   serial Data from the shift register chips and representing
   the state of those pins in an unsigned integer (or long).
*/
unsigned long readShiftRegisterValues();

bool isButtonClicked(unsigned long pinValues);

void displayPinValues(unsigned long pinValues);

void initButtonReader();

#endif