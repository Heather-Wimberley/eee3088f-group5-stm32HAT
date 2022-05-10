/*
 * sensors.c
 *
 *  Created on: May 10, 2022
 *      Author: kelvi
 */

#include "../inc/sensors.h"


void Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler, float freq){
	ProximitySensor_Start(i2cHandler);
	TempSensor_Start(hadc);

}


void Sensors_Stop(void){
	ProximitySensor_Stop();
	TempSensor_Stop();
}

// THis will get all measurements and store it in a pointer to sensor data struct
// User is responsible for creating this struct
// Will return 1 if there is an error
char Sensors_GetMeasurement(struct SensorData *out, int maxTime){
	if (TempSensor_HasStarted() != 1){
		return 1;
	}
	if (ProximitySensor_IsReady() != 1){
		return 1;
	}
	out->temp = TempSensor_GetMeasurement(maxTime);
	out->prox = ProximitySensor_GetSingleShotMeasurement();
	return 0;
}
