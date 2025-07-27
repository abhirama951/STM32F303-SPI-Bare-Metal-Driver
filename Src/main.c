#include "stm32f3xx.h"
#include <stdint.h>
#include "uart.h"
#include "adxl345.h"

int16_t vals[6];
int16_t reads[3];
float xg, yg, zg;

int main(){
	usart2_init((uint32_t)(8000000), (uint32_t)(115200));
	printf("USART Initialized\r\n");
	adxl345_init();
	for(int i=0; i<90000; i++);

	while(1){
		adxl345_read(reads, vals);

		xg = reads[0]*ADXL_CONST;
		yg = reads[1]*ADXL_CONST;
		zg = reads[2]*ADXL_CONST;

		printf("Acceleration values Xg = %f    Yg = %f    Zg = %f", xg, yg, zg);

		//Pseudo Delay
		for(int i=0; i<90000; i++);
		printf("\r\n");

	}
}
