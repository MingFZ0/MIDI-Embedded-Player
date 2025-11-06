#ifndef INC_PROJECT_UTIL_H_
#define INC_PROJECT_UTIL_H_

void clear_buffer(char *buffer, int size);
void display_menu();
void run_local_cmd(int state);
void run_command(char* buffer);
void time_countdown(struct sys_tick* systck, int re_vars[]);

#endif
