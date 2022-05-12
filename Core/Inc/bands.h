/*
 * bands.h
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include <stdint.h>

#ifndef INC_BANDS_H_
#define INC_BANDS_H_

void displayBand(int band_index);
void setBand(int band_index);
void blink(uint8_t r, uint8_t g, uint8_t b, uint8_t milis);
void fadeIN(uint8_t min, uint8_t max, uint8_t speed);
void fadeOUT(uint8_t min, uint8_t max, uint8_t speed);
//void setResistance(int band_index);

#endif /* INC_BANDS_H_ */
