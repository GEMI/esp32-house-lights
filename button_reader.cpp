#include "button_reader.h"

// https://playground.arduino.cc/Code/ShiftRegSN74HC165N/
// This is the input reading part
// How many 74HC165 Shift registers are you connecting?
#define SHIFT_REGISTER_COUNT 3
/* Width of data (how many ext lines). */
#define DATA_WIDTH SHIFT_REGISTER_COUNT * 8
/* Width of pulse to trigger the shift register to read and latch. */

#define PULSE_WIDTH_USEC 5
/* Optional delay between shift register reads. */
#define POLL_DELAY_MSEC 1
/* You will need to change the "int" to "long" If the
   SHIFT_REGISTER_COUNT is higher than 2.
*/
#define BYTES_VAL_T unsigned long

int PARALEL_LOAD_PIN  = 16;  // Connects to Parallel load pin GPIO16 / RX2
int CLOCK_ENABLE_PIN  = 17;  // Connects to Clock Enable pin GPIO17 / TX2
int DATA_PIN          = 18;  // Connects to the Q7 pin GPIO18
int CLOCK_PIN         = 19;  // Connects to the Clock pin GIO19

/* This function is essentially a "shift-in" routine reading the
   serial Data from the shift register chips and representing
   the state of those pins in an unsigned integer (or long).
*/
BYTES_VAL_T readShiftRegisterValues() {
  long bitVal;
  BYTES_VAL_T bytesVal = 0;

  /* Trigger a parallel Load to latch the state of the data lines */
  digitalWrite(CLOCK_ENABLE_PIN, HIGH);
  digitalWrite(PARALEL_LOAD_PIN, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);

  digitalWrite(PARALEL_LOAD_PIN, HIGH);
  digitalWrite(CLOCK_ENABLE_PIN, LOW);

  /* Loop to read each bit value from the serial out line
     of the SN74HC165N.
  */
  for (int i = 0; i < DATA_WIDTH; i++) {
    // Probably just create an array of bits
    bitVal = digitalRead(DATA_PIN);

    /* Set the corresponding bit in bytesVal. */
    bytesVal |= (bitVal << ((DATA_WIDTH - 1) - i));

    /* Pulse the Clock (rising edge shifts the next bit). */
    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(CLOCK_PIN, LOW);
  }

  return (bytesVal);
}

void displayPinValues(BYTES_VAL_T pinValues) {
  for (int i = 0; i < DATA_WIDTH; i++) {
    if ((pinValues >> i) & 1) {
      Serial.print("  Pin-");
      Serial.print(i);
      Serial.print(": ");
      Serial.print("HIGH");
    }
  }

  Serial.print("\r\n");
}

void initButtonReader() {
  pinMode(PARALEL_LOAD_PIN, OUTPUT);
  pinMode(CLOCK_ENABLE_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);

  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(PARALEL_LOAD_PIN, HIGH);
}

bool isButtonClicked(BYTES_VAL_T pinValues) {
    for (int i = 0; i < DATA_WIDTH; i++) {
      if ((pinValues >> i) & 1) {
        return true;
      }
    }
    return false;
}