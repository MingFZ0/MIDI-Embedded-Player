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

//void EXTI15_10_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(B1_Pin);
//	printf("%s\r\n", "Pressed");
//}
//
//void EXTI9_5_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(S1_Pin);
//	printf("%s\r\n", "Small Button Pressed");
//}

void blue_button_interrupt() {
	GPIO_Init();
}


