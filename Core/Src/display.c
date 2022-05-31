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

void displayBackgrounds()
{
    ST7735_FillRectangleFast(0,0,160,80,ST7735_COLOR565(10, 10, 10));
    ST7735_FillRectangleFast(0,64,160,16,ST7735_COLOR565(50, 50, 50));
    ST7735_FillRectangleFast(0,0,160,26,ST7735_COLOR565(50, 50, 50));
}

void displayFrequency(uint32_t freq)
{
    //if(band_index != lastBand + 1 )
    //{

        //ST7735_WriteString(1, 2, band[band_index].name, Font_16x26, ST7735_COLOR565(255, 179, 0), ST7735_COLOR565(50, 50, 50));
        // freq
       	sprintf(buff, "%d", freq);
       	ST7735_WriteString(0, 34, buff, Font_16x26, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
 //      	sprintf(buff, "%s - %s", band[band_index].NminFreq, band[band_index].NmaxFreq);
 //      	ST7735_WriteString(0, 46, buff, Font_11x18, ST7735_COLOR565(66, 186, 255), ST7735_COLOR565(10, 10, 10));
 //   }

}


