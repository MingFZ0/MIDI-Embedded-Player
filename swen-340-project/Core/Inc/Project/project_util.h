#ifndef INC_PROJECT_UTIL_H_
#define INC_PROJECT_UTIL_H_

#include <stdint.h>
#include "stm32l4xx.h"

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

void clear_buffer(char *buffer, int size);
void display_menu();
void run_local_cmd(int state);
void run_command(char* buffer);
void time_countdown(struct sys_tick* systck, int time_vars[]);
uint32_t get_count();

#endif
