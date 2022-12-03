/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "FX1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "PWM1.h"
#include "TU1.h"
#include "PWM2.h"
#include "TU2.h"
#include "PWM3.h"
#include "TU3.h"
#include "SS1.h"
#include "MCUC1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
#include "MK64F12.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#define DISTANCE_THRESHOLD 30
#define NUM_SENSORS 3

uint8_t IN[2];
uint8_t OUT[2];

volatile bool DataReceivedFlag = FALSE;

struct SensorData {
	uint8_t id;
	uint8_t distance;
	uint8_t occupied;
} sensors[NUM_SENSORS];

struct PWM_Config {
	uint32_t* mod;
	uint32_t* count;
	uint16_t temp;
} pwm_val[NUM_SENSORS];

LDD_TDeviceData* SS1_DeviceData;

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	/* Write your code here */
	SS1_DeviceData = SS1_Init(NULL);
	uint32_t delay;
	OUT[0] = 255;
	OUT[1] = 255;

	// Initialize all sensor ids, distances to zero, and occupied to false.
	for(uint8_t i = 0; i < NUM_SENSORS; i++) {
		sensors[i].id = i+1;
		sensors[i].distance = DISTANCE_THRESHOLD + 1;
		sensors[i].occupied = 0x00;
	}

	FX1_Init();

	pwm_val[0].mod = &FTM3_MOD;
	pwm_val[0].count = &FTM3_C6V;
	pwm_val[0].temp = FTM3_C6V;

	pwm_val[1].mod = &FTM0_MOD;
	pwm_val[1].count = &FTM0_C0V;
	pwm_val[1].temp = FTM0_C0V;

	pwm_val[2].mod = &FTM2_MOD;
	pwm_val[2].count = &FTM2_C0V;
	pwm_val[2].temp = FTM2_C0V;

	uint8_t min_index = 10;
	/* For example: for(;;) { } */

	for(;;) {
		// on each loop iteration, check if there is any sensor data to receive, update the appropriate sensor, update the LEDs.
		SS1_ReceiveBlock(SS1_DeviceData, IN, 2);
		SS1_SendBlock(SS1_DeviceData, OUT, 2);
		while(!DataReceivedFlag);
		DataReceivedFlag = FALSE;

		printf("Received ID: %x \n", IN[0]);
		printf("Received Distance: %x \n", IN[1]);

		for(uint8_t i = 0; i < NUM_SENSORS; i++) {
			if(sensors[i].id == IN[0]) {
					sensors[i].distance = IN[1];
					sensors[i].occupied = sensors[i].distance < DISTANCE_THRESHOLD ? 0x01: 0x00;
			}
			printf("ID: %d \n", sensors[i].id);
			printf("Distance: %d \n", sensors[i].distance);
			printf("Occupied: %d \n", sensors[i].occupied);

		}

		for(uint8_t i = 0; i < NUM_SENSORS; i++) {
			if(sensors[i].occupied == 0x00) {
				min_index = i;
				break;
			}
		}

		for(uint8_t i = 0; i < NUM_SENSORS; i++) {
			if(sensors[i].occupied == 0x01) {
				*(pwm_val[i].count) = *(pwm_val[i].mod);
			} else {
				if(i == min_index) {
					*(pwm_val[i].count) = *(pwm_val[i].mod)/2;
				} else {
					*(pwm_val[i].count) = 10;
				}
			}
		}

	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.4 [05.11]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
