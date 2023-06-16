#include "push_button.h"
#include "meter.h"
#include "display.h"
#include <MyFreeRTOS.h>


int main(void)
{
	/* Initialize heap for FreeRTOS and application */
	/* Add your code here! : DONE! */
	prvInitialiseHeap();
	/* End of your code! */
	PB_Init();
	MET_Init();
	DISP_Init();

	/* Start FreeRTOS scheduler */
	/* Add your code here! */
	vTaskStartScheduler();
	/* End of your code! */
	return 0;
}
