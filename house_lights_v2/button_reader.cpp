#include "button_reader.h"

// https://playground.arduino.cc/Code/ShiftRegSN74HC165N/
// This is the input reading part
/* Width of data (how many ext lines). 4 x 8 */
#define DATA_WIDTH 32
/* Width of pulse to trigger the shift register to read and latch. */

#define PULSE_WIDTH_USEC 5
/* Optional delay between shift register reads. */
#define POLL_DELAY_MSEC 1
#define BYTES_VAL_T unsigned long

int PARALEL_LOAD_PIN  = 16;  // Connects to Parallel load pin GPIO16 / RX2
int CLOCK_ENABLE_PIN  = 17;  // Connects to Clock Enable pin GPIO17 / TX2
int DATA_PIN          = 18;  // Connects to the Q7 pin GPIO18
int CLOCK_PIN         = 19;  // Connects to the Clock pin GIO19

/* This function is essentially a "shift-in" routine reading the
   serial Data from the shift register chips and representing
   the state of those pins in an unsigned integer (or long).
*/
BYTES_VAL_T readShiftRegisterValues()
{
  long bitVal;
  BYTES_VAL_T bytesVal = 0;

  digitalWrite(CLOCK_ENABLE_PIN, HIGH);
  digitalWrite(PARALEL_LOAD_PIN, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);
  digitalWrite(PARALEL_LOAD_PIN, HIGH);
  digitalWrite(CLOCK_ENABLE_PIN, LOW);

  for (int i = 0; i < DATA_WIDTH; i++)
  {
    bitVal = digitalRead(DATA_PIN);
    bytesVal |= (bitVal << ((DATA_WIDTH - 1) - i));
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