#ifndef __UART_H
#define __UART_H

#include "stm32f3xx.h"
#include <stdint.h>
#include <stdio.h>

void usart2_init(uint32_t sysclk, uint32_t baud_rate);
uint32_t set_usartdiv_over16(uint32_t sysclk, uint32_t baud_rate);
uint32_t set_usartdev_over8(uint32_t sysclk, uint32_t baud_rate);
void usart2_transmit(int ch);
char usart2_receive(void);

#endif
