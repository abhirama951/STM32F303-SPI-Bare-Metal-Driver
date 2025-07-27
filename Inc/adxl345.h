/*
 * adxl345.h
 *
 *  Created on: Jul 25, 2025
 *      Author: abhiram
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#define ADXL_CONST 0.0039

#include "stm32f3xx.h"
#include <stdint.h>
#include "spi.h"

void adxl345_init(void);
void adxl345_read(int16_t *reads, int16_t *vals);

#endif /* ADXL345_H_ */
