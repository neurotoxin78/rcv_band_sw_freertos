/*
 * bands.cpp
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include <stdio.h>
#include <bands.h>
#include "spi.h"
#include "st7735.h"
#include "fonts.h"

typedef struct
{
  const char *name;
  const char *minFreq; // Min. frequency value for the band (unit 0.01Hz)
  const char *maxFreq; // Max. frequency value for the band (unit 0.01Hz)
  const int	tics;
} Band;

Band band[5] = {
  {"FM  ", "80Mhz", "108Mhz",1}, // 100KHz to 1700KHz
  {"AM  ", "1600Khz", "3000KHz",120},
  {"SW1 ", "7.2Mhz", "10Mhz", 140},
  {"SW2 ", "10Mhz", "13Mhz",150},
  {"SW3 ", "14Mhz", "21Mhz",160},
};
// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
int currentBand = 0;                          // First band. For this case, AM is the current band.

void displayBand(int band_index)
{
    if(band_index != lastBand + 1 )
    {
        ST7735_FillRectangleFast(0,0,160,38,ST7735_COLOR565(10, 10, 10));
        ST7735_FillRectangleFast(0,38,160,2,ST7735_COLOR565(50, 50, 50));
        ST7735_WriteString(1, 1, band[band_index].name, Font_16x26, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(10, 10, 10));
        ST7735_WriteString(5, 26, "BAND", Font_7x10, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(10, 10, 10));
        ST7735_WriteString(75, 1, band[band_index].minFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
        ST7735_WriteString(75, 19, band[band_index].maxFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
    }

}

