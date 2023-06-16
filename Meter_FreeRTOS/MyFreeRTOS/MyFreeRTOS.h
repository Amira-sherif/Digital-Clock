#ifndef __MY_FREERTOS_H__
#define __MY_FREERTOS_H__

#include "FreeRTOS.h"
#include <task.h>
#include <timers.h>
#include <message_buffer.h>
#include <queue.h>
#include <semphr.h>
#include <meter.h>
#include <event_groups.h>


#define mainREGION_1_SIZE	8201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	7607

extern MessageBufferHandle_t xPB2MET_MessageBuffer;
#define mbPB2MET_LENGTH_BYTES	(50)

extern QueueHandle_t xMET2DISP_Queue;
#define qMET2DISP_LENGTH_MESSAGES	(10) 

typedef enum
{
	MET2DISP_UpdateMetertingData,
	MET2DISP_UpdateWindow,
}tMET2DISP_MessageType;

typedef union
{
	tMET_Window window;
	tMET_Measurement measurement;
}tMET2DISP_MessageData;

typedef struct
{
	tMET2DISP_MessageType type;
	tMET2DISP_MessageData data;
}tMET2DISP_Message;


extern EventGroupHandle_t xSyncEventGroup;
#define ebBIT_PB	(0x01)
#define ebBIT_MET	(0x02)
#define ebBIT_DISP	(0x04)
#define ebALL_SYNC_BITS (ebBIT_PB | ebBIT_MET | ebBIT_DISP) 


void  prvInitialiseHeap(void);
void vAssertCalled(unsigned long ulLine, const char* const pcFileName);


#endif 
