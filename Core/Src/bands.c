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
#include <string.h>

#define START_RESISTANCE_KOM  57;

typedef struct
{
  const char *name;
  const char *WminFreq; // Min. frequency value for the band (unit 0.01Hz)
  const char *WmaxFreq; // Max. frequency value for the band (unit 0.01Hz)
  const char *NminFreq; // Min. frequency value for the band (unit 0.01Hz)
  const char *NmaxFreq; // Max. frequency value for the band (unit 0.01Hz)
  const uint32_t ticks;
} Band;

Band band[5] = {
  {"FM  ", "80", "108Mhz", "", "",0}, // 100KHz to 1700KHz
  {"AM  ", "1.6", "3MHz", "", "",120},
  {"SW1 ", "2.3", "10Mhz", "2.3", "2.4Mhz", 140},
  {"SW2 ", "3.2", "7.6Mhz", "3.2", "3.4Mhz",150},
  {"SW3 ", "3.2", "10Mhz", "3.9", "4Mhz",160},

};
// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
int currentBand = 0;                          // First band. For this case, AM is the current band.
char buff[20];
uint32_t ticks;

void displayBand(int band_index)
{
    if(band_index != lastBand + 1 )
    {
        ST7735_FillRectangleFast(0,0,160,80,ST7735_COLOR565(10, 10, 10));
        ST7735_FillRectangleFast(0,65,160,15,ST7735_COLOR565(50, 50, 50));
        ST7735_FillRectangleFast(0,0,160,26,ST7735_COLOR565(50, 50, 50));
        ST7735_WriteString(1, 1, band[band_index].name, Font_16x26, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(50, 50, 50));
        //ST7735_WriteString(5, 26, "BAND", Font_7x10, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(10, 10, 10));
        // Band Frequencies Wide
       	//ST7735_WriteString(5, 26, "    ", Font_7x10, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(10, 10, 10));
       	sprintf(buff, "W:%s - %s", band[band_index].WminFreq, band[band_index].WmaxFreq);
       	ST7735_WriteString(0, 26, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
       	sprintf(buff, "N:%s - %s", band[band_index].NminFreq, band[band_index].NmaxFreq);
       	ST7735_WriteString(0, 45, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
    }

}

void setBand(int band_index)
{
	ticks = band[band_index].ticks + START_RESISTANCE_KOM;
	sprintf(buff, "R:%ld", ticks);
	ST7735_WriteString(0, 62, buff, Font_11x18, ST7735_COLOR565(165, 214, 2), ST7735_COLOR565(50, 50, 50));
}
