/*
 * prox.c
 *
 *  Created on: May 6, 2022
 *      Author: kelvi
 */

#include "../inc/prox.h"

VL6180Dev_t vl6180Handler = (PROX_I2C_ADDRESS & 0x7f) << 1;

SensorErrorType ProximitySensor_Start(I2C_HandleTypeDef* i2cHandler) {
	// For now, the handler is equal to the i2c address of the chip
	// Workaround for not having the i2c handler for hal in vl6180 code
	RegisterI2CHandler(i2cHandler);
	//We need to switch on the GPIO pin to change the state of the machine
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(1);
	VL6180_WaitDeviceBooted(vl6180Handler);
	// Init Chip
	int status;
	status = VL6180_InitData(vl6180Handler);
	if (status ==  CALIBRATION_WARNING) {
		// Error Handling here
		return PROX_CALIBRATION_ERROR;
	}
	status = VL6180_Prepare(vl6180Handler);
	if ( status != 0){
		return PROX_INIT_ERROR;
	}




	// Need to wait according to data sheet
	HAL_Delay(0.4);

	//Device should be ready for commands now
	VL6180_UpscaleSetScaling(vl6180Handler, 1);
	//TODO Explore offset and cross talk calibration
	return OK;

}




void EnsureHandlersValid(void){
	assert_param(vl6180Handler != NULL);
	//Assert param is already in this function
	GetI2CHandler();
}


SensorErrorType ProximitySensor_GetSingleShotMeasurement(int32_t* result) {
	VL6180_RangeData_t Range;
	VL6180_RangePollMeasurement(vl6180Handler, &Range);
	if (Range.errorStatus != 0){
		//Error Handler
		return PROX_MEASURE_ERROR;
	}

	// Range measurement available in Range.mm
	(*result) = Range.range_mm;
	return OK;

}

SensorErrorType ProximitySensor_Stop(void){
	//This will send the proximity sensor into hardware standby mode
	VL6180_ClearAllInterrupt(vl6180Handler);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	return OK;
}

SensorErrorType ProximitySensor_IsReady(void) {
	I2C_HandleTypeDef* handler = GetI2CHandler();

	if (HAL_I2C_IsDeviceReady(handler, (0x29 & 0x7f) << 1, 3, 1) != 0){
		return PROX_NOT_RESPONDING;
	}
	return OK;
}
