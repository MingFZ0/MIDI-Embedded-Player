#ifndef _COMMANDS_H_
#define	_COMMANDS_H_

struct sys_tick;
void run_play();
void run_stop();
void run_pause(struct sys_tick* systck, int count, int time, int re_vars[]);

#endif
