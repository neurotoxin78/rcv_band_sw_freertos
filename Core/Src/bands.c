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

typedef struct
{
  const char *name;
  const char *WminFreq; // Min. frequency value for the band (unit 0.01Hz)
  const char *WmaxFreq; // Max. frequency value for the band (unit 0.01Hz)
  const char *NminFreq; // Min. frequency value for the band (unit 0.01Hz)
  const char *NmaxFreq; // Max. frequency value for the band (unit 0.01Hz)
  const int	ticks;
} Band;

Band band[5] = {
  {"FM  ", "80MHz", "108Mhz", "", "",1}, // 100KHz to 1700KHz
  {"AM  ", "1600KHz", "3000KHz", "", "",120},
  {"SW1 ", "2.3", "10Mhz", "2.3", "2.4Mhz", 140},
  {"SW2 ", "3.2", "7.6Mhz", "3.2", "3.4Mhz",150},
  {"SW3 ", "3.2", "10Mhz", "3.9", "4Mhz",160},

};
// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
int currentBand = 0;                          // First band. For this case, AM is the current band.
char buff[20];

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
        if (band[band_index].name == "FM  ")
        {
            ST7735_WriteString(75, 1, band[band_index].WminFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(50, 50, 50));
            ST7735_WriteString(75, 19, band[band_index].WmaxFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(50, 50, 50));
        }else if (band[band_index].name == "AM  ")
        {
        	ST7735_WriteString(75, 1, band[band_index].WminFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(50, 50, 50));
        	ST7735_WriteString(75, 19, band[band_index].WmaxFreq, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(50, 50, 50));
        } else
        {
        	//ST7735_WriteString(5, 26, "    ", Font_7x10, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(10, 10, 10));
        	sprintf(buff, "W:%s - %s", band[band_index].WminFreq, band[band_index].WmaxFreq);
        	ST7735_WriteString(0, 26, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
        	sprintf(buff, "N:%s - %s", band[band_index].NminFreq, band[band_index].NmaxFreq);
        	ST7735_WriteString(0, 45, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
        }

    }

}

