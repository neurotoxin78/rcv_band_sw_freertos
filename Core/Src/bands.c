/*
 * bands.cpp
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include <stdio.h>
#include <bands.h>
#include "main.h"
#include "spi.h"
#include "st7735.h"
#include "fonts.h"
#include <string.h>
#include "ARGB.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#define START_RESISTANCE_KOM  57
#define CS_PIN (GPIO_PIN_15)
#define INC_PIN (GPIO_PIN_13)
#define UD_PIN  (GPIO_PIN_14)

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
  {"FM  ", "80", "108Mhz", "DeEmph", "50uS",10}, // 100KHz to 1700KHz
  {"AM  ", "1.6", "3MHz", "", "",20},
  {"SW1 ", "W:2.3", "10Mhz", "N:2.3", "2.4Mhz", 30},
  {"SW2 ", "W:3.2", "7.6Mhz", "N:3.2", "3.4Mhz",40},
  {"SW3 ", "W:3.2", "10Mhz", "N:3.9", "4Mhz",50},

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
        ST7735_FillRectangleFast(0,64,160,16,ST7735_COLOR565(50, 50, 50));
        ST7735_FillRectangleFast(0,0,160,26,ST7735_COLOR565(50, 50, 50));
        ST7735_WriteString(1, 2, band[band_index].name, Font_16x26, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(50, 50, 50));
        // freq
       	sprintf(buff, "%s - %s", band[band_index].WminFreq, band[band_index].WmaxFreq);
       	ST7735_WriteString(0, 29, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
       	sprintf(buff, "%s - %s", band[band_index].NminFreq, band[band_index].NmaxFreq);
       	ST7735_WriteString(0, 46, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
    }

}



void initDP(int CHIP)
{
	HAL_GPIO_WritePin(GPIOB, CHIP, GPIO_PIN_SET); // SET CS
}


void setResistanceUP(int CHIP, uint8_t resistance, bool save)
{
	HAL_GPIO_WritePin(GPIOB, UD_PIN, GPIO_PIN_SET); // HIGH
	HAL_GPIO_WritePin(GPIOB, INC_PIN, GPIO_PIN_SET); //HIGH
	delay_us(300);
	HAL_GPIO_WritePin(GPIOB, CHIP, GPIO_PIN_RESET); // LOW
    for (int i = 0; i < resistance; i++)
    {
        HAL_GPIO_WritePin(GPIOB, INC_PIN, GPIO_PIN_RESET); //LOW
        //vTaskDelayMS(300);
    	delay_us(3);
        HAL_GPIO_WritePin(GPIOB, INC_PIN, GPIO_PIN_SET); //HIGH
        //vTaskDelayMS(300);
    	delay_us(3);
    }
    if (save)
    {
    	HAL_GPIO_WritePin(GPIOB, INC_PIN, GPIO_PIN_SET); //HIGH
    	//delay_us(300);
    }else
    {
    	HAL_GPIO_WritePin(GPIOB, INC_PIN, GPIO_PIN_RESET); //LOW
    	//delay_us(300);
    }
	delay_us(300);
    HAL_GPIO_WritePin(GPIOB, CHIP, GPIO_PIN_SET); // HIGH
	delay_us(300);
}

void setBand(int band_index)
{
	ticks = band[band_index].ticks; //- START_RESISTANCE_KOM;
	sprintf(buff, "R:%ld", ticks);
	ST7735_WriteString(0, 64, buff, Font_11x18, ST7735_COLOR565(165, 214, 2), ST7735_COLOR565(50, 50, 50));
	//setResistanceUP(CS_PIN, ticks, false);
}

void blink(uint8_t r, uint8_t g, uint8_t b, uint8_t milis)
{
    ARGB_FillRGB(r, g, b); // Fill all the strip with Red
    while (!ARGB_Show());
    osDelay(milis);
    ARGB_Clear(); // Clear stirp
    while (ARGB_Show() != ARGB_OK); // Update - Option 1
}
