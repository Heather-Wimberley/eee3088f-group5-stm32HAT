#ifndef TEMP_SENSOR
#define TEMP_SENSOR
#include "stm32f0xx_hal.h"
#include  "types.h"


extern ADC_HandleTypeDef *ADCState;
extern char SensorState;




SensorErrorType TempSensor_Start(ADC_HandleTypeDef *hadc);

SensorErrorType TempSensor_GetMeasurement(int maxTime, float* result);

SensorErrorType TempSensor_HasStarted(void);

SensorErrorType TempSensor_Stop(void);

void EnsureHandleValid(void);






#endif
