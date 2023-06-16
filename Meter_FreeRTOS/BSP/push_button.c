#include "push_button.h"
#include <pc.h>
#include <MyFreeRTOS.h>
#include <FreeRTOSConfig.h>

#define N_PUSH_BUTTONS	(4)
#define N_SAMPLES	(2)
#define OLD_SAMPLE	(0)
#define NEW_SAMPLE	(1)

/* Key states */
#define PB_PRESSED	(1)
#define PB_RELEASED	(0)

/* Keys' codes */
#define C_KEY		(0x43)
#define F_KEY		(0x46)
#define RIGHT_KEY	(0x27)
#define LEFT_KEY	(0x25)

typedef enum
{
	PB_Clear,
	PB_Function,
	PB_Right,
	PB_Left
}tPB;

typedef enum
{
	PB_Released,
	PB_PrePressed,
	PB_Pressed,
	PB_PreReleased
}tPB_State;

/* Push button info type */
typedef struct
{
	uint8_t samples[N_SAMPLES];
	tPB_State state;
}tPB_Info;

static tPB_Info prvPB_Info[4];

static void prvPB_InitPB(tPB pb, tPB_State pb_state);
static void prvPB_Task(void* pvParameters);
static void prvPB_Update(void);

void PB_Init(void)
{
	/* Initialize push buttons */
	prvPB_InitPB(PB_Clear, PB_Released);
	prvPB_InitPB(PB_Function, PB_Released);
	prvPB_InitPB(PB_Right, PB_Released);
	prvPB_InitPB(PB_Left, PB_Released);

	/* Create push buttons task */
	/* Add your code here!: DONE! */
	xTaskCreate(prvPB_Task, "Button Task", 200, NULL, PB_PRIORITY, NULL);
	/* End of your code! */

	/* Create push buttons task to meter task message buffer */
	/* Add your code here!: DONE! */
	xPB2MET_MessageBuffer = xMessageBufferCreate(mbPB2MET_LENGTH_BYTES);
	/* End of your code! */

	/* Create synch event group */
	/* Add your code here!: TO BE MODIFIED! */
	xSyncEventGroup = xEventGroupCreate();		/* Should the handle be global and passed to other tasks???*/
	/* End of your code! */

}

static void prvPB_InitPB(tPB pb, tPB_State pb_state)
{
	prvPB_Info[pb].state = pb_state;
	switch (pb_state)
	{
	case PB_Released:
	case PB_PreReleased:
		prvPB_Info[pb].samples[OLD_SAMPLE] = PB_RELEASED;
		prvPB_Info[pb].samples[NEW_SAMPLE] = PB_RELEASED;
		break;
		break;
	case PB_Pressed:
	case PB_PrePressed:
		prvPB_Info[pb].samples[OLD_SAMPLE] = PB_PRESSED;
		prvPB_Info[pb].samples[NEW_SAMPLE] = PB_PRESSED;
		break;
	default: /* Should not be here */
		break; 
	}
}

static void prvPB_Task(void* pvParameters)
{
	TickType_t xNextWakeTime;
	const TickType_t xBlockTime = pdMS_TO_TICKS(PB_PERIOD_MS);
	
	/* Initialization */
	(void)pvParameters;

	/* Sync all tasks start  */
	/* Add your code here!: DONE! */
	//xEventGroupSync(xSyncEventGroup, ebBIT_PB, ebALL_SYNC_BITS, xBlockTime);
	/* End of your code! */

	/* Get system tick count */
	/* Add your code here!: DONE! */
	xNextWakeTime = xTaskGetTickCount();
	/* End of your code! */

	for (;;)
	{
		prvPB_Update();
		/* delay for PB_PERIOD_MS*/
		/* Add your code here!: DONE! */
		vTaskDelayUntil(&xNextWakeTime, pdMS_TO_TICKS(PB_PERIOD_MS));
		/* End of your code! */
	}
}

static void prvPB_Update(void)
{
	uint8_t	pb_index;
	uint8_t message = 0;
	static uint8_t counter = 0;

	/* Update samples */
	for (pb_index = 0; pb_index < N_PUSH_BUTTONS; pb_index++)
	{
		prvPB_Info[pb_index].samples[OLD_SAMPLE] = prvPB_Info[pb_index].samples[NEW_SAMPLE];
	}

	prvPB_Info[PB_Clear].samples[NEW_SAMPLE] = 		(GetKeyState(C_KEY) & 0x8000)? PB_PRESSED : PB_RELEASED;
	prvPB_Info[PB_Function].samples[NEW_SAMPLE] = 	(GetKeyState(F_KEY) & 0x8000)? PB_PRESSED : PB_RELEASED;
	prvPB_Info[PB_Right].samples[NEW_SAMPLE] = 		(GetKeyState(RIGHT_KEY) & 0x8000)? PB_PRESSED : PB_RELEASED;
	prvPB_Info[PB_Left].samples[NEW_SAMPLE] = 		(GetKeyState(LEFT_KEY) & 0x8000)? PB_PRESSED : PB_RELEASED;

	/* Update state and press time */
	for (pb_index = 0; pb_index < N_PUSH_BUTTONS; pb_index++)
	{
		switch (prvPB_Info[pb_index].state)
		{
		case PB_Released:
			if ((prvPB_Info[pb_index].samples[OLD_SAMPLE] == PB_PRESSED) &&
				(prvPB_Info[pb_index].samples[NEW_SAMPLE] == PB_PRESSED))
			{
				prvPB_Info[pb_index].state = PB_PrePressed;
			}
			else
			{
			}
			break;
		case PB_PrePressed:
			if (prvPB_Info[pb_index].samples[NEW_SAMPLE] == PB_PRESSED)
			{
				prvPB_Info[pb_index].state = PB_Pressed;
				message = pb_index;
			}
			else
			{
					
					
			}
			break;
		case PB_Pressed:
			if ((prvPB_Info[pb_index].samples[OLD_SAMPLE] == PB_RELEASED) &&
				(prvPB_Info[pb_index].samples[NEW_SAMPLE] == PB_RELEASED))
			{
				prvPB_Info[pb_index].state = PB_PreReleased;
			}
			else
			{
			}
			break;
		case PB_PreReleased:
			if (prvPB_Info[pb_index].samples[NEW_SAMPLE] == PB_RELEASED)
			{
				prvPB_Info[pb_index].state = PB_Released;
			}
			else
			{
			}
			break;
		default: /* Should not be here */
			break;
		}
	}	

	/* Update and send messages to meter task */
	/* Add your code here! */
	if (counter == 2){
		message = 2;
	}
	xMessageBufferSend(xPB2MET_MessageBuffer, &message, sizeof(message), 10);
	counter ++;
	/* End of your code! */
}


