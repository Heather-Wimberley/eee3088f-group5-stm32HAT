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

/*
    SensorMode. Indicates whether the sensor was started in polling or interrupt mode.
    0 is not started. 1 is polling. 2 is interrupt
*/
extern char SensorMode;

SensorErrorType Sensors_Start(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler);
SensorErrorType Sensors_Stop();
SensorErrorType Sensors_GetMeasurement(struct SensorData *out, int maxTime);
SensorErrorType Sensors_Start_IT(ADC_HandleTypeDef *hadc, I2C_HandleTypeDef* i2cHandler, int freq);


#endif /* SRC_SENSORS_SENSORS_H_ */
