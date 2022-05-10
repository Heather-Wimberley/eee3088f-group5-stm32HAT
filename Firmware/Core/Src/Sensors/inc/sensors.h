/*
 * sensors.h
 *
 *  Created on: May 10, 2022
 *      Author: kelvi
 */

#ifndef SRC_SENSORS_SENSORS_H_
#define SRC_SENSORS_SENSORS_H_
#include "temp.h"
#include "prox.h"


struct SensorData {
	float temp;
	int32_t prox;
};


void Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler, float freq);
void Sensors_Stop();
char Sensors_GetMeasurement(struct SensorData *out, int maxTime);


#endif /* SRC_SENSORS_SENSORS_H_ */
