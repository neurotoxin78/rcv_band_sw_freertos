/*
 * NeoPixel.h
 *
 *  Created on: May 9, 2022
 *      Author: neuro
 */
#include <stdint.h>
#include "tim.h"
#include "dma.h"

#ifndef INC_NEOPIXEL_H_
#define INC_NEOPIXEL_H_

#include <stdint.h>

void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_render();


#endif /* INC_NEOPIXEL_H_ */
