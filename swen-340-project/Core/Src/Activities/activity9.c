/**
 * activity9.c
 *
 * Write a function that prints on/off when the blue button is pressed
 */

#include "GPIO.h"
#include <stdint.h>
#include "printf.h"
#include "stm32l4xx.h"

void blue_button() {
	GPIO_Init();

	while (1) {
		if (B1_GPIO_Port->IDR & B1_Pin) {
			printf("off\r\n");
		}
		else {
			printf("on\r\n");
		}
	}
}
