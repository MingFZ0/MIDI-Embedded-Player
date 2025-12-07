/*
 * project.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_PROJECT_H_
#define INC_PROJECT_H_

#include "stm32l4xx.h"

void flip_operation_mode();
int get_current_mode();
int get_play_state();
int get_pause_state();
void set_state_pause();
void set_state_play();
void set_state_clear();
void set_current_song(int song);
int get_current_song();

void small_button_check();
void run_project ();
void display_menu();

#endif /* INC_PROJECT_H_ */
