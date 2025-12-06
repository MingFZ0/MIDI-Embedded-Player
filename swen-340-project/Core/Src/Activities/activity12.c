#include "adc.h"
#include "dac.h"
#include "adc_demo.h"
#include "systick.h"

#include <stdio.h>


void run_activity12() {
	DAC_Init ();
	DAC_Start ();
	DAC_Set_Value (3000);


}
