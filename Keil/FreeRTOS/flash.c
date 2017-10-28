#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "Board_LED.h"

#include "MKW41Z4.h"

#define LED_FREQUENCY_1_MS 1000
#define LED_FREQUENCY_2_MS 2000

static void prvFlashLED1Task (void *pvParameters);
static void prvFlashLED2Task (void *pvParameters);
	
int main(void)
{
	//Initialise LEDs.
	LED_Initialize();

	//Create the two tasks. 
	xTaskCreate( prvFlashLED1Task,							//The function that implements the task.
							 "Flash1", 											//The text name assigned to the task - for debug only. 
							 configMINIMAL_STACK_SIZE, 			//The size of the stack to allocate to the task.
							 NULL, 													//The parameter passed to the task. 
							 tskIDLE_PRIORITY + 1, 					//The priority assigned to the task.
							 NULL );												//The task handle is not required, so NULL is passed.

 
	xTaskCreate( prvFlashLED2Task,							//The function that implements the task.
							 "Flash2", 											//The text name assigned to the task - for debug only.
							 configMINIMAL_STACK_SIZE, 			//The size of the stack to allocate to the task.
							 NULL, 													//The parameter passed to the task.
							 tskIDLE_PRIORITY + 2, 					//The priority assigned to the task.
							 NULL );												//The task handle is not required, so NULL is passed. 
 
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
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
 
		//Turn on an LED. 
		LED_On(0);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
		
		//Turn off an LED.
    LED_Off(0);
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
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_RATE_MS ); 
 
		//Turn on an LED.
		LED_On(3);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_RATE_MS ); 
		
		//Turn off an LED.
    LED_Off(3);
	} 
} 
