#ifndef light_manager
#define light_manager

#include <Arduino.h>

void toggleBit(byte*, int, int);

void initLightManager(byte []);

void writeDataToRelays(byte []);

#endif