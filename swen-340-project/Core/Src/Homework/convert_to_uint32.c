#include "printf.h"
#include <stdio.h>
#include <stdint.h>

uint32_t convert_to_uint32(uint8_t* p_value) {
	uint32_t value1 = p_value[0] << (3*8);	// 0x 78 00 00 00
	uint32_t value2 = p_value[1] << (2*8);  // 0x 00 56 00 00
	uint32_t value3 = p_value[2] << (1*8); 	// 0x 00 00 34 00
	uint32_t value4 = p_value[3]; 			// 0x 00 00 00 12
	uint32_t result = value1 + value2 + value3 + value4;
//	printf("	p_value after: %d\r\n", result);
	return result;
}
