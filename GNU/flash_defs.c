#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "MKW41Z4.h"

#define LED_FREQUENCY_1_MS 1000
#define LED_FREQUENCY_2_MS 2000

#define STD_RED_LED_PIN 0
#define STD_RED_LED_PORT PORTB_BASE
#define STD_RED_LED_GPIO GPIOB_BASE

#define RGB_RED_LED_PIN 1
#define RGB_RED_LED_PORT PORTC_BASE
#define RGB_RED_LED_GPIO GPIOC_BASE

static void prvFlashLED1Task (void *pvParameters);
static void prvFlashLED2Task (void *pvParameters);
	
int main(void)
{
	//Initialise LEDs.
		//Enable the clock gate to the PORTn modules.
		SIM->SCGC5 |= ( (1U << SIM_SCGC5_PORTB_SHIFT) |
					    (1U << SIM_SCGC5_PORTC_SHIFT) );

		//Initialise the ports.
			//PORTB for the regular red LED.
			((PORT_Type *)STD_RED_LED_PORT)->PCR[STD_RED_LED_PIN]  = (1U << 8);
			((PORT_Type *)STD_RED_LED_PORT)->ISFR           	  &= (1U << STD_RED_LED_PIN);
			
			//PORTC for the RGB red LED.
			((PORT_Type *)RGB_RED_LED_PORT)->PCR[RGB_RED_LED_PIN]  = (1U << 8);
			((PORT_Type *)RGB_RED_LED_PORT)->ISFR           	  &= (1U << RGB_RED_LED_PIN);

		//Initialise the GPIO pins.
			//GPIOB for the regular red LED.
			((GPIO_Type *)STD_RED_LED_GPIO)->PDOR |=  (1U << STD_RED_LED_PIN);
			((GPIO_Type *)STD_RED_LED_GPIO)->PDDR |=  (1U << STD_RED_LED_PIN);
			
			//GPIOB for the regular red LED.
			((GPIO_Type *)RGB_RED_LED_GPIO)->PDOR |=  (1U << RGB_RED_LED_PIN);
			((GPIO_Type *)RGB_RED_LED_GPIO)->PDDR |=  (1U << RGB_RED_LED_PIN);

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

//Task to make the regular red LED flash.
static void prvFlashLED1Task( void *pvParameters ) 
{ 
	while(1) 
	{ 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
 
		//Turn on the regular red LED. 
		((GPIO_Type *)STD_RED_LED_GPIO)->PCOR = (1U << STD_RED_LED_PIN);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
		
		//Turn off an LED.
		((GPIO_Type *)STD_RED_LED_GPIO)->PSOR = (1U << STD_RED_LED_PIN);
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
		((GPIO_Type *)RGB_RED_LED_GPIO)->PCOR = (1U << RGB_RED_LED_PIN);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_RATE_MS ); 
		
		//Turn off an LED.
		((GPIO_Type *)RGB_RED_LED_GPIO)->PSOR = (1U << RGB_RED_LED_PIN);
	} 
} 
