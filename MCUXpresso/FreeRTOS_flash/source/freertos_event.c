/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_FREQUENCY_1_MS 1000
#define LED_FREQUENCY_2_MS 2000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void prvFlashLED1Task (void *pvParameters);
static void prvFlashLED2Task (void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    //Initialise LEDs.
    SIM->SCGC5 |= ((1U << SIM_SCGC5_PORTA_SHIFT) |
                   (1U << SIM_SCGC5_PORTB_SHIFT) |
                   (1U << SIM_SCGC5_PORTC_SHIFT)  );
    LED_RED_INIT(1);
    LED_BLUE_INIT(1);

    //Create the two tasks.
	xTaskCreate( prvFlashLED1Task,					//The function that implements the task.
				 "Flash1", 							//The text name assigned to the task - for debug only.
				 configMINIMAL_STACK_SIZE, 			//The size of the stack to allocate to the task.
				 NULL, 								//The parameter passed to the task.
				 tskIDLE_PRIORITY + 1, 				//The priority assigned to the task.
				 NULL );							//The task handle is not required, so NULL is passed.


	xTaskCreate( prvFlashLED2Task,					//The function that implements the task.
				 "Flash2", 							//The text name assigned to the task - for debug only.
				 configMINIMAL_STACK_SIZE, 			//The size of the stack to allocate to the task.
				 NULL, 								//The parameter passed to the task.
				 tskIDLE_PRIORITY + 2, 				//The priority assigned to the task.
				 NULL );							//The task handle is not required, so NULL is passed.

	//Start the tasks and timer running.
	vTaskStartScheduler();

	//Loop forever.
	while(1)
	{
		;
	}
}

//Task to make the faster LED flash.
static void prvFlashLED1Task( void *pvParameters )
{
	while(1)
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_PERIOD_MS );

		//Turn on an LED.
		LED_RED_ON();

		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_PERIOD_MS );

		//Turn off an LED.
		LED_RED_OFF();
	}
}

//Task to make the slower LED flash.
static void prvFlashLED2Task( void *pvParameters )
{
	while(1)
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_PERIOD_MS );

		//Turn on an LED.
		LED_BLUE_ON();

		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_PERIOD_MS );

		//Turn off an LED.
		LED_BLUE_OFF();
	}
}

