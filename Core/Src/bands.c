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
  const uint64_t minFreq; // Min. frequency value for the band (unit 0.01Hz)
  const uint64_t maxFreq; // Max. frequency value for the band (unit 0.01Hz)
} Band;

Band band[5] = {
  {"1", 100000, 1700000}, // 100KHz to 1700KHz
  {"2", 120000000, 135000000},
  {"3", 135000000, 160000000},
  {"4", 135000000, 160000000},
  {"5", 135000000, 160000000},
};
// Calculate the last element position (index) of the array band
const int lastBand = (sizeof band / sizeof(Band)) - 1; // For this case will be 26.
int currentBand = 0;                          // First band. For this case, AM is the current band.
uint32_t freqMin = 0;
uint32_t freqMax = 0;
char *freqName;
char buff[16];

void setBand(int counter)
{
	if(counter == 1)
	{
		currentBand += 1;
	} else
	{
		currentBand -=1;
	}
}

void displayBand(int band_index)
{
    if(band_index != lastBand + 1 )
    {
    	// Check border
    	freqMin = band[band_index].minFreq;
    	freqMax = band[band_index].maxFreq;
    	freqName = band[band_index].name;
        ST7735_FillScreen(ST7735_BLACK);
        //snprintf(buff, sizeof(buff), "%02d", freqName);
        ST7735_WriteString(15, 0, freqName, Font_16x26, ST7735_COLOR565(255, 187, 51), ST7735_BLACK);
        snprintf(buff, sizeof(buff), "%05d", freqMin);
        ST7735_WriteString(20, 60, buff, Font_7x10, ST7735_GREEN, ST7735_BLACK);
        snprintf(buff, sizeof(buff), "%05d", freqMax);
        ST7735_WriteString(85, 60, buff, Font_7x10, ST7735_GREEN, ST7735_BLACK);
    }

}

