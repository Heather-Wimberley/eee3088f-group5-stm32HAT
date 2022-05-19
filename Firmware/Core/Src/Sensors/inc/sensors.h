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
#include "types.h"




SensorErrorType Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler);
SensorErrorType Sensors_Stop();
SensorErrorType Sensors_GetMeasurement(struct SensorData *out, int maxTime);


#endif /* SRC_SENSORS_SENSORS_H_ */
