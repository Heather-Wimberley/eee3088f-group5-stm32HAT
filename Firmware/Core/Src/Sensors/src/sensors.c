/*
 * sensors.c
 *
 *  Created on: May 10, 2022
 *      Author: kelvi
 */

#include "../inc/sensors.h"


SensorErrorType Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler){
	SensorErrorType result = ProximitySensor_Start(i2cHandler);
	if (result != OK){
		return result;
	}
	result = TempSensor_Start(hadc);
	if (result != OK){
			return result;
	}
	return OK;

}


SensorErrorType Sensors_Stop(void){
	SensorErrorType result = ProximitySensor_Stop();
	if (result != OK){
		return result;
	}
	return TempSensor_Stop();
}

// THis will get all measurements and store it in a pointer to sensor data struct
// User is responsible for creating this struct
// Will return 1 if there is an error
SensorErrorType Sensors_GetMeasurement(struct SensorData *out, int maxTime){
	if (TempSensor_HasStarted() != 1){
		return 1;
	}
	if (ProximitySensor_IsReady() != OK){
		return 1;
	}
	float temp = 0;
    SensorErrorType result = TempSensor_GetMeasurement(1, &temp);
    if (result != OK){
    	return result;
    }
    int32_t prox = 0;


	out->temp = temp;
	result = ProximitySensor_GetSingleShotMeasurement(&prox);
	out->prox = prox;
	return result;
}


//Error Handling Section




