/*
 * project.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_PROJECT_H_
#define INC_PROJECT_H_

void set_state_pause();
void set_state_play();
void set_state_clear();
void set_current_song(int song);
int get_current_song();

void run_project ();
void display_menu();

#endif /* INC_PROJECT_H_ */
