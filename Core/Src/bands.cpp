/*
 * bands.cpp
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include "bands.h"
#include <stdint.h>

typedef struct
{
  const char *name;
  uint32_t minFreq; // Min. frequency value for the band (unit 0.01Hz)
  uint32_t maxFreq; // Max. frequency value for the band (unit 0.01Hz)
} Band;

Band band[] = {
  {"LW/MW", 100000, 1700000}, // 100KHz to 1700KHz
  {"  SW1", 1700000, 3500000},
  {"  SW2", 3500000, 4000000},
  {"  SW3", 4000000, 7000000},
  {"  40m", 7000000, 7300000},   // 7MHz to 7.3 MHz  (Amateur 40m)
  {"  41m", 7300000, 9000000},   // 41m
  {"  31m", 9000000, 10000000},  // 31m
  {"  30m", 10000000, 11000000}, // 10 MHz to 11 MHz (Amateur 30m)
  {"  25m", 11000000, 14000000}, // 25 and 22 meters
  {"  20m", 14000000, 15000000}, // 14MHz to 15Mhz (Amateur 20m)
  {"  19m", 15000000, 17000000}, // 19m
  {"  16m", 17000000, 18000000}, // 16m
  {"  15m", 18000000, 20000000}, // 18MHz to 20Mhz (Amateur and comercial 15m)
  {"  13m", 20000000, 21350000}, // 20MHz to 22Mhz (Amateur and comercial 15m/13m)
  {" SW14", 21350000, 22000000},
  {" SW15", 22350000, 24980000},
  {"  12m", 24880000, 24990000}, // 24.88MHz to 24.99MHz (Amateur 12m)
  {" SW17", 24990000, 26000000},
  {" SW18", 26000000, 28000000},
  {"  10m", 28000000, 30000000}, // 28MHz to 30MHz (Amateur 10M)
  {" VHF1", 30000000, 50000000},
  {" VHF2", 50000000, 54000000},
  {" VHF3", 54000000, 86000000},
  {"   FM", 86000000, 108000000},  // Comercial FM
  {" VHF4", 108000000, 120000000}, // 108MHz to 160MHz
  {" VHF5", 120000000, 135000000},
  {" VHF6", 135000000, 160000000}
};

// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
volatile int currentBand = 23;                          // First band. For this case, AM is the current band.


