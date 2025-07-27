#include "uart.h"

int __io_putchar(int ch){
	usart2_transmit(ch);
	return ch;
}

void usart2_init(uint32_t sysclk, uint32_t baud_rate){
	/**********Configuring GPIO Pins to USART Mode*/
	/****Enabling Clock Access to GPIOA and GPIOB****/
	RCC->AHBENR |= ((1U<<17)|(1U<<18));

	/****Setting pins PA2 and PA3 to alternate function Mode****/
	GPIOA->MODER |= ((1U<<5)|(1U<<7));
	GPIOA->MODER &= ~((1U<<4)|(1U<<6));

	/****Setting alterate function mode to AF7****/
	GPIOA->AFR[0] |= ((1U<<8)|(1U<<9)|(1U<<10));
	GPIOA->AFR[0] |= ((1U<<12)|(1U<<13)|(1U<<14));
	GPIOA->AFR[0] &= ~((1U<<11)|(1U<<15));

	/****Setting pin PB13 to output mode****/
	GPIOB->MODER |= (1U<<26);
	GPIOB->MODER &= ~(1U<<27);

	/****Enabling clock access to USART2****/
	RCC->APB1ENR |= (1U<<17);

	/***********Configuring USART module**********/
	/****Resetting USART Control Registers****/
	USART2->CR1 = 0x00;
	USART2->CR2 = 0x00;
	USART2->CR3 = 0x00;

	/****No of bits in frame set to 8 bits****/

	/****To set Baud Rate****/
	/****Setting BRR Register for baud rate****/
	USART2->BRR = set_usartdiv_over16(sysclk, baud_rate);

	/****Enabling UART Module****/
	USART2->CR1 |= (1U<<0);

	/****Enable transmit function in UART****/
	USART2->CR1 |= (1U<<3);

	/****Enable receiver function in UART****/
	USART2->CR1 |= (1U<<2);
}

uint32_t set_usartdiv_over16(uint32_t sysclk, uint32_t baud_rate){
	/****Baud Rate = fclk/USARTDIV for standard USART with oversampling by 16****/
	return (sysclk/baud_rate);
}

uint32_t set_usartdev_over8(uint32_t sysclk, uint32_t baud_rate){
	/****Baud Rate = (2*fclk)/USARTDIV for standard USART with oversampling by 16****/
	return ((2*sysclk)/baud_rate);
}

void usart2_transmit(int ch){

	/****Wait till TDR is empty****/
	while(!(USART2->ISR & (1U<<7)));

	/****Place data into TDR****/
	USART2->TDR = ch&0xFF;
}

char usart2_receive(void){

	/****Wait while RDR is empty****/
	while(!(USART2->ISR & (1U<<5)));
	return (USART2->RDR);
}
