#include "pc.h"
#include "display.h"
#include <stdint.h>
#include "meter.h"
#include <string.h>
#include <stdio.h>
#include <MyFreeRTOS.h>

static uint8_t prvDISP_DisplayRow = 1;
static uint8_t prvDISP_DisplayColumn = 0;
static tMET_Measurement prvDISP_MeterMeasurement;
static tMET_Config prvDISP_MeterConfiguration;


static char prvDISP_DisplayString[2][17] = {
                                            "                ",
                                            "                ",
                                           };
static char prvDISP_Strings[26][17] =
{
    "",					    // 0
    "Power Draw:",			// 1
    "Recording Time:",		// 2
    "Energy Consumed:",		// 3
    "Predicted Usage:",		// 4
    "CO2 Emissions: ",		// 5
    "Cost per kWh: ",			// 6
    "Billing Days:",			// 7
    "Max:",				// 8
    "Min:",				// 9
    "W",					// 10
    "kWh",					// 11
    "$",					// 12
    "Kg CO2",				// 13
    "/bill",				// 14
    "/year",				// 15
    "/hour",				// 16
    "tonne/y",				// 17
    "Kg/kWh",				// 18
    "PRESS CLEAR TO",		// 19
    "START RECORDING",		// 20
    "ENERGY METER 2.3",		// 21
    "Enhanced",			// 22
    "CLEAR",				// 23
    "kW/hour",				// 24
    "Watts"				// 25
};

static void prvDISP_Start(void);
static void prvDisp_UpdateDisplayString();
static void prvDISP_Task(void* pvParameters);

void DISP_Init(void)
{
    /* Get meter configuration */
    MET_GetConfiguration(&prvDISP_MeterConfiguration);

     prvDISP_Start();

    /* Creat display queue */
     /* Add your code here!: DONE!*/
     xMET2DISP_Queue= xQueueCreate( qMET2DISP_LENGTH_MESSAGES, sizeof( uint8_t ) );
     /* End of your code! */

    /* Create display task */
     /* Add your code here!: DONE!*/
	xTaskCreate(prvDISP_Task, "Disptask", 200, NULL, DISP_PPRIORITY, NULL);

     /* End of your code! */
}

static void prvDISP_Task(void * pvParameters)
{
    tMET2DISP_Message message;

    (void)pvParameters;
    /* Sync all tasks start  */
    /* Add your code here! */
    //xEventGroupSync(xSyncEventGroup, ebBIT_DISP, ebALL_SYNC_BITS, portMAX_DELAY);
    /* End of your code! */

    for (;;)
    {
        /* Wait for message from meter task  */
        /* Add your code here! */
        xQueueReceive(xMET2DISP_Queue, &message, 100);
        /* End of your code! */

        if (message.type == MET2DISP_UpdateWindow)
        {
            prvDISP_DisplayRow = message.data.window.row;
            prvDISP_DisplayColumn = message.data.window.column;
        }
        else if (message.type == MET2DISP_UpdateMetertingData)
        {
            prvDISP_MeterMeasurement.calc_hours = message.data.measurement.calc_hours;
            prvDISP_MeterMeasurement.float_cost = message.data.measurement.float_cost;
            prvDISP_MeterMeasurement.kwh = message.data.measurement.kwh;
            prvDISP_MeterMeasurement.kwh_per_hour = message.data.measurement.kwh_per_hour;
            prvDISP_MeterMeasurement.kwh_per_hour_cost = message.data.measurement.kwh_per_hour_cost;
            prvDISP_MeterMeasurement.watts = message.data.measurement.watts;
            prvDISP_MeterMeasurement.watts_max = message.data.measurement.watts_max;
            prvDISP_MeterMeasurement.watts_min = message.data.measurement.watts_min;
        }

        prvDisp_UpdateDisplayString();
    }
}


static void prvDISP_Start(void)
{
    PC_DispStr(0, 0, "                         FreeRTOS, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED);
    PC_DispStr(0, 1, "   Original version by Richard Barry, PC Utility port by Werner Zimmermann      ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr(0, 2, "                    Example designed by Amr A. Abdel-Naby                       ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr(0, 3, "                              Meter FreeRTOS                                    ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 5, " To change meter display:                                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 6, " ------------------------                                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 7, " c  - Clear Meter Display                                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 8, " f  - Change Display Level                                                      ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 9, " -> - Display Next Window                                                       ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 10, " <- - Display Previous Window                                                   ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 14, "                              Meter Display                                     ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_WHITE);
    PC_DispStr(0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_RED);
    /*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
    /*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

static void prvDisp_UpdateDisplayString(void)
{
    
    uint8_t string_length = 0;
    uint8_t string_padding_length = 0; 
    /* Update display string */
    switch (prvDISP_DisplayRow)
    {
    case 1: /* Current power */
        strcpy(prvDISP_DisplayString[0], prvDISP_Strings[1]);
        if (prvDISP_DisplayColumn %3 == 0)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.1f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.watts, prvDISP_Strings[25]);
        }
        else if (prvDISP_DisplayColumn % 3 == 1)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.1f %s", prvDISP_Strings[8], prvDISP_MeterMeasurement.watts_max, prvDISP_Strings[10]);
        }
        else
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.1f %s", prvDISP_Strings[9], prvDISP_MeterMeasurement.watts_min, prvDISP_Strings[10]);
        }
        break;
    case 2: /* Recording time */
        strcpy(prvDISP_DisplayString[0], prvDISP_Strings[2]);
        sprintf(prvDISP_DisplayString[1], "%s %01.2f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.calc_hours, "hours");
        break;
    case 3: /* Energy consumed */
        strcpy(prvDISP_DisplayString[0], prvDISP_Strings[3]);
        if (prvDISP_DisplayColumn % 3 == 0)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.3f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.kwh, prvDISP_Strings[11]);
        }
        else if (prvDISP_DisplayColumn % 3 == 1)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.2f %s", prvDISP_Strings[12], (double)prvDISP_MeterMeasurement.kwh * prvDISP_MeterMeasurement.float_cost, prvDISP_Strings[0]);
        }
        else
        {
            sprintf(prvDISP_DisplayString[1], "%s %1.2f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.kwh *(float) prvDISP_MeterConfiguration.co2_rate/100, prvDISP_Strings[13]);
        }
        break;
    case 4: /* Predicted usage */
        strcpy(prvDISP_DisplayString[0], prvDISP_Strings[4]);
        if (prvDISP_DisplayColumn % 4 == 0)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.2f %s", prvDISP_Strings[12], (double)prvDISP_MeterMeasurement.kwh_per_hour_cost*24*365, prvDISP_Strings[15]);
        }
        else if (prvDISP_DisplayColumn % 4 == 1)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.2f %s", prvDISP_Strings[12], (double)prvDISP_MeterMeasurement.kwh_per_hour_cost*24* prvDISP_MeterConfiguration.bill_cycle, prvDISP_Strings[14]);
        }
        else if (prvDISP_DisplayColumn % 4 == 2)
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.2f %s", prvDISP_Strings[12], prvDISP_MeterMeasurement.kwh_per_hour_cost, prvDISP_Strings[16]);
        }
        else
        {
            sprintf(prvDISP_DisplayString[1], "%s %01.3f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.kwh_per_hour, prvDISP_Strings[24]);
        }
        break;
    case 5: /* CO2 emission  */
        strcpy(prvDISP_DisplayString[0], prvDISP_Strings[5]);
        sprintf(prvDISP_DisplayString[1], "%s %01.3f %s", prvDISP_Strings[0], prvDISP_MeterMeasurement.kwh_per_hour * 24 *365 * (float) prvDISP_MeterConfiguration.co2_rate/100000, prvDISP_Strings[17]);
        break;
    }

    /* Pad strings with spaces */
    string_length = (uint8_t)strlen(prvDISP_DisplayString[0]);
    for (uint8_t i = string_length; i < 16; i++)
    {
        prvDISP_DisplayString[0][i] = ' ';
    }
    prvDISP_DisplayString[0][16] = '\0';

    string_length = (uint8_t)strlen(prvDISP_DisplayString[1]);
    for (uint8_t i = string_length; i < 16; i++)
    {
        prvDISP_DisplayString[1][i] = ' ';
    }
    prvDISP_DisplayString[1][16] = '\0';
    
    /* Print display string */
    PC_DispStr(28, 15, prvDISP_DisplayString[0], DISP_FGND_WHITE + DISP_BGND_RED);
    PC_DispStr(28, 16, prvDISP_DisplayString[1], DISP_FGND_WHITE + DISP_BGND_RED);
}
