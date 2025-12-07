#include "stm32l476xx.h"
#include "printf.h"
#include <stdint.h>

uint32_t paseDelay (uint8_t* delay) {

	int value = 0;

	while (1) {
		uint8_t *current_byte = delay;
		value = value << 7;
		value |= (*current_byte & 0x7F);

		if (!(*delay & 0b10000000)) {
			printf("Total value: %d\r\n", value);
			break;
		}
		delay++;

	}

	printf("\r\n");
	return value;
}

void hw8_run() {
	printf("%s", "First:\r\n");
	uint8_t first[] = {0x3B,0x80,0x27,0x00};
	paseDelay(first);

	printf("%s", "Second:\r\n");
	uint8_t second[] = {0x83,0x18,0x91,0x54,0x64};
	paseDelay(second);

	printf("%s", "Third:\r\n");
	uint8_t third[] = {0xC5,0x92,0x74,0xE3,0x74,0x12};
	paseDelay(third);

	printf("%s", "Fourth:\r\n");
	uint8_t fourth[] = {0x90,0x82,0x93,0x64,0xD3,0x89};
	paseDelay(fourth);
}
