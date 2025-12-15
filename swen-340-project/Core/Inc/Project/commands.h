#ifndef _COMMANDS_H_
#define	_COMMANDS_H_

#include "project_util.h"
#include <stdint.h>

void run_play();
void run_stop();
int run_next(int current_song);
void run_pause(struct sys_tick* systck, int count, int time, int re_vars[]);
uint32_t get_song_tempo();

#endif
