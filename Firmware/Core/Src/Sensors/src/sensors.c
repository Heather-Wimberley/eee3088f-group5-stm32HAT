/*
 * sensors.c
 *
 *  Created on: May 10, 2022
 *      Author: kelvi
 */

#include "../inc/sensors.h"


/* @description
 * Initialize the Sensors to start collecting data
 *
 * @arguments
 * @hadc is the handler of the adc configured by the user. Instructions about specific
 * ports can be found in the readme
 * @i2cHandler is the handler of the i2c configuration defined by the user.
 * @returns SensorErrorType
 *
 *
 * */

SensorErrorType Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler){
	if (SensorMode != 0 || SensorMode != 1){
		// Sensor configured in interrupt mode
		return SENSOR_INIT_ERROR;
	}
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

/* @description
 *
 * Places both sensors in a stand by state
 * @arguments
 * @returns SensorErrorType
 */
SensorErrorType Sensors_Stop(void){
	SensorErrorType result = ProximitySensor_Stop();
	if (result != OK){
		return result;
	}
	return TempSensor_Stop();
}
/*
 * @description
 * This will get all measurements and store it in a pointer to sensor data struct
 *
 * @arguments
 * @{SensorData} out - a pointer to the struct in which the results from measurements will be stored
 * @{int} maxTime - the maximum time the sensors should take to get information, if the time is exceeded, the function will return an error
 *
 *
 *
 */


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


SensorErrorType Sensors_Start_IT(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler, int freq){
	if (SensorMode != 0 || SensorMode != 2){
		// Sensor configured in polling mode
		return SENSOR_INIT_ERROR;
	}
	SensorErrorType result = ProximitySensor_Start_IT(i2cHandler, freq);
	if (result != OK){
		return result;
	}
	result = TempSensor_Start(hadc);
	if (result != OK){
			return result;
	}
	SensorMode = 2
	return OK;


}




