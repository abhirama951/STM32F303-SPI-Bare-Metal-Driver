/*
 * spi.h
 *
 *  Created on: Jul 25, 2025
 *      Author: abhiram
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f3xx.h"
#include <stdint.h>

void spi2_gpio_init(void);
void spi2_config_init(void);
void spi2_read_reg(int16_t *data, int8_t reg_addr);
void spi2_burst_read_reg(int16_t *data, int8_t reg_addr, uint8_t len);
void spi2_write_reg(int16_t *data, int8_t reg_addr);
void pb1_cs_enable(void);
void pb1_cs_disable(void);

#endif /* SPI_H_ */
