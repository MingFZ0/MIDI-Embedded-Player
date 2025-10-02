#include "activity6.h"
#include "systick.h"
#include <stdint.h>

struct sys_tick {
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
};

struct sys_tick* systck;

// This function is to Initialize SysTick registers
void init_systick()
{
	systck = (struct sys_tick*) 0xE000E010;
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	systck->CSR = 0;
	// Set the LOAD (RVR) to 8 million to give us a 100 milliseconds timer.
	systck->RVR = 8000000;
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	systck->CSR |= 1<<2;
	// Set the enable bit in the CTRL (CSR) to start the timer.
	systck->CSR |= 1;

}

// This fuction is to create delay using SysTick timer counter
void delay_systick()
{

	// Create a for loop that loops 10 times
	for (int loop = 0; loop < 10; loop++) {

		// Inside that for loop check the COUNTFLAG bit in the CTRL (CSR)
		int bool = 0;
		while (bool == 0) {
			if (systck->CSR & (1<<16)) {bool = 1;}
			//When that bit is set, exit this inner loop
		}

	}
}
