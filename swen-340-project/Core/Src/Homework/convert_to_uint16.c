#include "printf.h"
#include <stdio.h>
#include <stdint.h>

uint16_t convert_to_uint16(uint8_t* p_value) {
	uint16_t value = *p_value << 8;
//	printf("	p_value before: %d\r\n", value);
	value |= p_value[1];
//	printf("	p_value after: %d\r\n", value);
	return value;
}
