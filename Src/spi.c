#include "spi.h"

#define AHBENR_GPIOB		(1U<<18)
#define SPI2_SCK 			(1U<<13)
#define SPI2_MISO			(1U<<14)
#define SPI2_MOSI			(1U<<15)
#define SPI2_CSN			(1U<<1)
#define APB1ENR_SPI2		(1U<<14)
#define CR1_CPHA			(1U<<0)
#define CR1_CPOL 			(1U<<1)
#define CR1_SSM				(1U<<9)
#define CR1_SSI				(1U<<8)
#define CR1_MSTR			(1U<<2)
#define CR1_SPE				(1U<<6)
#define ADXL_MB				(1U<<14)
#define ADXL_RWN			(1U<<15)
#define SR_BSY				(1U<<7)
#define SR_TXE				(1U<<1)
#define SR_RXNE				(1U<<0)

/*Void function to initialize GPIO pins for SPI2
 * Params : None
 * Return : None
 * Pins : PB13-->SCK PB14-->MISO PB15-->MOSI PB1-->CSN*/
void spi2_gpio_init(void){
	//Enable clock access to GPIOB
	RCC->AHBENR |= AHBENR_GPIOB;

	//Set pins to alternate function mode
	GPIOB->MODER &= ~(1U<<26);
	GPIOB->MODER |= (1U<<27);

	GPIOB->MODER &= ~(1U<<28);
	GPIOB->MODER |= (1U<<29);

	GPIOB->MODER &= ~(1U<<30);
	GPIOB->MODER |= (1U<<31);

	//Set pin speed to medium
	GPIOB->OSPEEDR |= (1U<<26);
	GPIOB->OSPEEDR &= ~(1U<<27);

	GPIOB->OSPEEDR |= (1U<<28);
	GPIOB->OSPEEDR &= ~(1U<<29);

	GPIOB->OSPEEDR |= (1U<<30);
	GPIOB->OSPEEDR &= ~(1U<<31);

	//Set alternate function to AF5
	GPIOB->AFR[1] |= ((1U<<20)|(1U<<22));
	GPIOB->AFR[1] &= ~((1U<<21)|(1U<<23));

	GPIOB->AFR[1] |= ((1U<<24)|(1U<<26));
	GPIOB->AFR[1] &= ~((1U<<25)|(1U<<27));

	GPIOB->AFR[1] |= ((1U<<28)|(1U<<30));
	GPIOB->AFR[1] &= ~((1U<<29)|(1U<<31));

	//Set pin 1 to output mode
	GPIOB->MODER |= (1U<<2);
	GPIOB->MODER &= ~(1U<<3);

	//Set default state of pin to HIGH
	GPIOB->ODR |= SPI2_CSN;
}

/*Void function enable clock access to SPI2 and configure it in correct clock mode and frequency
 * Params : None
 * Return : None*/
void spi2_config_init(void){
	//Enable clock access to SPI2
	RCC->APB1ENR |= APB1ENR_SPI2;

	/*Configuring control register 1*/
	//Reset Control Register 1
	SPI2->CR1 = 0x0000;

	//Set serial clock baud rate to 125KHz
	SPI2->CR1 |= ((1U<<5)|(1U<<4)|(1U<<3));

	//Set clock polarity as CPOL = 1, CPHA = 1
	SPI2->CR1 |= (CR1_CPHA | CR1_CPOL);

	//2 line unidirectional full duplex communication set at reset

	//MSB First communication enabled at reset

	//Circular Redundancy Check is disabled at reset

	//Enable software slave management
	SPI2->CR1 |= (CR1_SSM | CR1_SSI);

	//Enable master mode of operation
	SPI2->CR1 |= CR1_MSTR;

	//Configuring control register 2
	//Resetting control register 2
	SPI2->CR2 = 0x0000;

	//Set transfer length to 16 bits
	SPI2->CR2 |= ((1U<<8)|(1U<<9)|(1U<<10)|(1U<<11));

	//Slave Select Output Disabled
	//Communication set to motorola mode
	//NSS Pulse Management Disabled
	//FIFO Register Threshold to 16 bit

	//Enable SPI Peripheral
	SPI2->CR1 |= CR1_SPE;
}

void spi2_read_reg(int16_t *data, int8_t reg_addr){

	//Create payload containing address from which data has to be read
	int16_t payload = ((reg_addr<<8)|(ADXL_RWN));

	//Wait till SPI Data Register is empty
	while(!(SPI2->SR & SR_TXE));

	SPI2->DR = payload;

	//Wait till SPI receives data
	while(!(SPI2->SR & SR_RXNE));
	//Read dummy data from peripheral device
	(void)(SPI2->DR);

	//Send dummy data
	while(!(SPI2->SR & SR_TXE));
	SPI2->DR = 0xFF;

	while(!(SPI2->SR & SR_RXNE));

	//Read data back
	*data = SPI2->DR;
}


void spi2_burst_read_reg(int16_t *data, int8_t reg_addr, uint8_t len){

	int16_t payload = ((reg_addr<<8)|(ADXL_RWN)|(ADXL_MB));

	//Wait till SPI Data Register is empty
	while(!(SPI2->SR & SR_TXE));

	SPI2->DR = payload;

	while(len--){
		//Wait till SPI receives data
		while(!(SPI2->SR & SR_RXNE));

		//Read data back
		*data++ = SPI2->DR;

		SPI2->DR = 0;
	}
}


void spi2_write_reg(int16_t *data, int8_t reg_addr){

	int16_t payload = ((*data<<0)|(reg_addr<<8));
	payload &= ~ADXL_RWN;

	//Wait till SPI lines are idle
	while(!(SPI2->SR & SR_TXE));

	//Write data to send to register
	SPI2->DR = payload;

	//Wait till SPI lines are idle
	while(SPI2->SR & SR_BSY);
}


void pb1_cs_enable(void){
	GPIOB->ODR &= ~SPI2_CSN;
	for(int i=0; i<9000; i++);
}


void pb1_cs_disable(void){
	for(int i=0; i<9000; i++);
	GPIOB->ODR |= SPI2_CSN;
}
