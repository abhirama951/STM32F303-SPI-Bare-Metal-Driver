#include "adxl345.h"

#define ADXL345_ADDR		(0x53)
#define POWER_CTLR			(0x2D)
#define DATA_FORMATR		(0x31)
#define ADXL_X0				(0x32)

void adxl345_init(void){
	int16_t init_dat[2] = {0x08, 0x01};
	//Emable FPU
	SCB->CPACR |= ((3U<<20)|(3U<<22));

	//Initialize SPI2
	spi2_gpio_init();
	spi2_config_init();

	pb1_cs_enable();

	//Initialize measure mode in SPI
	spi2_write_reg(&init_dat[0], (int8_t)(POWER_CTLR));

	//Initialize for 4g limits
	spi2_write_reg(&init_dat[1], (int8_t)(DATA_FORMATR));

	pb1_cs_disable();
}

void adxl345_read(int16_t *reads, int16_t *vals){

	pb1_cs_enable();

	spi2_burst_read_reg(vals, (uint8_t)(ADXL_X0), 6);

	pb1_cs_disable();

	reads[0] = (vals[0]|(vals[1]<<8));
	reads[1] = (vals[2]|(vals[3]<<8));
	reads[2] = (vals[4]|(vals[5]<<8));

	for(uint8_t i=0; i<6; i++){
		vals[i] = 0;
	}
}
