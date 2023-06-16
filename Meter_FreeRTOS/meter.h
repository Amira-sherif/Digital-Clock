#ifndef __METER_H__
#define __METER_H__

#include <stdint.h>

/* Define metering task priority */
/* Add your code here!: DONE! */
#define MET_PRIORITY		(1)
/* End of your code! */



typedef struct
{
    int16_t zero;
    int16_t gain;
    uint16_t cost; //$
    uint8_t co2_rate;
    uint8_t bill_cycle; //days 
}tMET_Config;

typedef struct
{
    float watts;
    float watts_max;
    float watts_min;
    float kwh;
    float kwh_per_hour;
    float calc_hours;
    float kwh_per_hour_cost;
    float float_cost;
}tMET_Measurement;

typedef struct
{
    uint8_t row;
    uint8_t column;
}tMET_Window;

void MET_Init(void);
void MET_GetConfiguration(tMET_Config * configuration);



#endif
