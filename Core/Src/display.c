/*
 * bands.cpp
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include <stdio.h>
#include "display.h"
#include "main.h"
#include "spi.h"
#include "st7735.h"
#include "fonts.h"
#include <string.h>
#include "gpio.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

char buff[20];

uint16_t light_color = ST7735_COLOR565(125, 160, 175);
uint16_t dark_color = ST7735_COLOR565(40, 80, 130);

void displayBackgrounds() {
	/* backgriund */
	ST7735_FillRectangleFast(0, 0, 160, 80, light_color);
	/* down string */
	ST7735_FillRectangleFast(0, 64, 160, 16, dark_color);
	/* up string */
	ST7735_FillRectangleFast(0, 0, 160, 20, dark_color);
	/* labels */
	ST7735_WriteString(1, 6, "BAND",Font_7x10, light_color, dark_color);
	ST7735_WriteString(72, 6,"STEP",Font_7x10, light_color, dark_color);
    ST7735_WriteString(2, 35, "VFO", Font_7x10, dark_color, light_color);
    ST7735_WriteString(2, 43, " Hz", Font_7x10, dark_color, light_color);
}

void displayBand(int band)
{
	ST7735_WriteString(35, 2, band, Font_11x18, light_color, dark_color);
}

void displayStep(int step)
{
	ST7735_WriteString(104, 2, step, Font_11x18, light_color, dark_color);
}

void displayFrequency(uint32_t freq) {
	uint16_t th, dig, mi;
	th = freq%1000;
	dig = (freq / 1000)%1000;
	mi = (freq / 1000000);
	sprintf(buff, "%02d.%03d",mi, dig);
	ST7735_WriteString(26, 32, buff, Font_16x26, dark_color, light_color);
	sprintf(buff, "%03d",th);
	ST7735_WriteString(125, 32, buff, Font_11x18, dark_color, light_color);
}

