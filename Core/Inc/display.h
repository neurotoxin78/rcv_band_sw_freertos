/*
 * bands.h
 *
 *  Created on: May 6, 2022
 *      Author: neuro
 */
#include <stdint.h>

#ifndef INC_BANDS_H_
#define INC_BANDS_H_

void displayFrequency(uint32_t freq);
void displayBackgrounds();
void displayBand(const char * band);
void displayStep(const char * step);
void blk_pwm_setvalue(uint16_t value);
void setBacklight(uint8_t percent);

#endif /* INC_BANDS_H_ */
