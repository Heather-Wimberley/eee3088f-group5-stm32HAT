/*
 * prox.h
 *
 *  Created on: May 6, 2022
 *      Author: kelvi
 */

#ifndef SRC_SENSORS_VL6180API_PROX_H_
#define SRC_SENSORS_VL6180API_PROX_H_
#include "vl6180_api.h"
#include "stm32f0xx_hal.h"


#define PROX_I2C_ADDRESS 0x29

extern VL6180Dev_t vl6180Handler;


void ProximitySensor_Start(I2C_HandleTypeDef* i2cHandler);

void ProximitySensor_Stop(void);
char ProximitySensor_IsReady(void);

int32_t ProximitySensor_GetSingleShotMeasurement(void);


#endif /* SRC_SENSORS_VL6180API_PROX_H_ */
