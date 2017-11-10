#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

//LED frequencies.
#define LED_FREQUENCY_1_MS 1000
#define LED_FREQUENCY_2_MS 2000

// Clock gating control addresses/values.
#define PORT_SCGC_ADDR 	  0x40048038u 	//SIM base address (0x40047000u) + SCGC5 offset (0x1038u).
#define PORTB_SCGC_ENABLE (1 << 10)		//1 << SIM_SCGC5_PORTB_SHIFT.
#define PORTC_SCGC_ENABLE (1 << 11)		//1 << SIM_SCGC5_PORTC_SHIFT.

//Regular red LED addresses/pins. Port B.
#define STD_RED_LED_PIN 0
#define STD_RED_LED_PORT_BASE_ADDR 0x4004A000u
#define STD_RED_LED_GPIO_BASE_ADDR 0x400FF040u

//RGB red LED addresses/pins. Port C.
#define RGB_RED_LED_PIN 1
#define RGB_RED_LED_PORT_BASE_ADDR 0x4004B000u
#define RGB_RED_LED_GPIO_BASE_ADDR 0x400FF080u

//PORT address offsets.
#define PCR_BASE_OFFSET    0x00u
#define PCR_OFFSET_PER_PIN 0x04u
#define ISFR_OFFSET		   0xA0u

//GPIO address offsets.
#define PDOR_OFFSET 0x00u
#define PSOR_OFFSET 0x04u
#define PCOR_OFFSET 0x08u
#define PDDR_OFFSET 0x14u

static void prvFlashLED1Task (void *pvParameters);
static void prvFlashLED2Task (void *pvParameters);
	
int main(void)
{
	//Initialise LEDs.
		//Enable the clock gate to the PORTn modules.
		uint32_t *PORT_SCGC = (uint32_t*)PORT_SCGC_ADDR;
		*PORT_SCGC |= (uint32_t)(PORTB_SCGC_ENABLE | PORTC_SCGC_ENABLE);

		//Initialise the ports.
			//PORTB for the regular red LED.
			uint32_t *std_red_LED_PORT_PCR  = (uint32_t*)(STD_RED_LED_PORT_BASE_ADDR + PCR_BASE_OFFSET 
														  + (STD_RED_LED_PIN * PCR_OFFSET_PER_PIN));	//Address of Port B Pin Control Register for the LED pin.
			uint32_t *std_red_LED_PORT_ISFR = (uint32_t*)(STD_RED_LED_PORT_BASE_ADDR + ISFR_OFFSET);	//Address of Port B Interrupt Status Flag Register.
			*std_red_LED_PORT_PCR   = (1U << 8);														//Configure the regular red LED pin as GPIO.
			*std_red_LED_PORT_ISFR &= (1U << STD_RED_LED_PIN);											//If the interrupt for the LED pin was already enabled, keep it enabled and disable all others on the port.
			
			//PORTC for the RGB red LED.
			uint32_t *rgb_red_LED_PORT_PCR  = (uint32_t*)(RGB_RED_LED_PORT_BASE_ADDR + PCR_BASE_OFFSET 
														  + (RGB_RED_LED_PIN * PCR_OFFSET_PER_PIN));	//Address of Port C Pin Control Register for the LED pin.
			uint32_t *rgb_red_LED_PORT_ISFR = (uint32_t*)(RGB_RED_LED_PORT_BASE_ADDR + ISFR_OFFSET);	//Address of Port C Interrupt Status Flag Register.
			*rgb_red_LED_PORT_PCR   = (1U << 8);														//Configure the RGB red LED pin as GPIO.
			*rgb_red_LED_PORT_ISFR &= (1U << RGB_RED_LED_PIN);											//If the interrupt for the LED pin was already enabled, keep it enabled and disable all others on the port.
			
		//Initialise the GPIO pins.
			//GPIOB for the regular red LED.
			uint32_t *std_red_LED_GPIO_PDOR = (uint32_t*)(STD_RED_LED_GPIO_BASE_ADDR + PDOR_OFFSET);	//Address of Port B Data Output Register.
			uint32_t *std_red_LED_GPIO_PDDR = (uint32_t*)(STD_RED_LED_GPIO_BASE_ADDR + PDDR_OFFSET);	//Address of Port B Data Direction Register.
			*std_red_LED_GPIO_PDOR |=  (1U << STD_RED_LED_PIN);											//Set the output of the regular red LED pin low.
			*std_red_LED_GPIO_PDDR |=  (1U << STD_RED_LED_PIN);											//Configure the regular red LED pin as an output.
			
			//GPIOC for the RGB red LED.
			uint32_t *rgb_red_LED_GPIO_PDOR = (uint32_t*)(RGB_RED_LED_GPIO_BASE_ADDR + PDOR_OFFSET);	//Address of Port C Data Output Register.
			uint32_t *rgb_red_LED_GPIO_PDDR = (uint32_t*)(RGB_RED_LED_GPIO_BASE_ADDR + PDDR_OFFSET);	//Address of Port C Data Direction Register.
			*rgb_red_LED_GPIO_PDOR |=  (1U << RGB_RED_LED_PIN);											//Set the output of the RGB red LED pin low.
			*rgb_red_LED_GPIO_PDDR |=  (1U << RGB_RED_LED_PIN);											//Configure the RGB red LED pin as an output.

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
	//Calculate the addresses of the Port Clear Output Register and Port Set Output Register for PORTB.
	uint32_t *std_red_LED_PCOR = (uint32_t*)(STD_RED_LED_GPIO_BASE_ADDR + PCOR_OFFSET);
	uint32_t *std_red_LED_PSOR = (uint32_t*)(STD_RED_LED_GPIO_BASE_ADDR + PSOR_OFFSET);

	while(1) 
	{ 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
 
		//Turn on the regular red LED. 
		*std_red_LED_PCOR = (1U << STD_RED_LED_PIN);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_1_MS / portTICK_RATE_MS ); 
		
		//Turn off the regular red LED.
		*std_red_LED_PSOR = (1U << STD_RED_LED_PIN);
	} 
} 

//Task to make the RGB red LED flash.
static void prvFlashLED2Task( void *pvParameters ) 
{ 	 
	//Calculate the addresses of the Port Clear Output Register and Port Set Output Register for PORTC.
	uint32_t *rgb_red_LED_PCOR = (uint32_t*)(RGB_RED_LED_GPIO_BASE_ADDR + PCOR_OFFSET);
	uint32_t *rgb_red_LED_PSOR = (uint32_t*)(RGB_RED_LED_GPIO_BASE_ADDR + PSOR_OFFSET);

	while(1) 
	{ 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_RATE_MS ); 
 
		//Turn on the RGB red LED.
		*rgb_red_LED_PCOR = (uint32_t)(1U << RGB_RED_LED_PIN);
 
		/* Place this task in the blocked state until it is time to run again. 
		The block time is specified in ticks, the constant used converts ticks 
		to ms.  While in the Blocked state this task will not consume any CPU 
		time. */ 
		vTaskDelay( LED_FREQUENCY_2_MS / portTICK_RATE_MS ); 
		
		//Turn off the RGB red LED.
		*rgb_red_LED_PSOR = (uint32_t)(1U << RGB_RED_LED_PIN);
	} 
} 
