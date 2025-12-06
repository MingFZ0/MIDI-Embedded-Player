//Create a clock that prints once a second using interrupts
//Steps:
//	Configure Systick handler for interrupts
//	Write interrupt handler
// 	Print every second

#include "systick.h"
#include "printf.h"
#include <stdint.h>

static int count = 0;


//void SysTick_Handler() {
//	count++;
//}
//
//uint32_t get_count() {return count;}

void activity10_run() {
	printf("r\n");
	init_systick();

	while (1) {
		if ((count & 1024) == 0) {
				printf("%d\r", count >> 10);
			}
		printf("%d\r", count);
	}
}
