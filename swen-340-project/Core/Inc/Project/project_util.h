#ifndef INC_PROJECT_UTIL_H_
#define INC_PROJECT_UTIL_H_

void clear_buffer(char *buffer, int size);
void display_menu();
void run_command(char* buffer);
void time_countdown(struct sys_tick* systck, int count, int time, int re_vars[]);

#endif
