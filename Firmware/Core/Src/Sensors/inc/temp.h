#ifndef TEMP_SENSOR
#define TEMP_SENSOR
#include "stm32f0xx_hal.h"


extern ADC_HandleTypeDef *ADCState;
extern char SensorState;




void TempSensor_Start(ADC_HandleTypeDef *hadc);

float TempSensor_GetMeasurement(int maxTime);

char TempSensor_HasStarted(void);

void TempSensor_Stop(void);

void EnsureHandleValid(void);






#endif
