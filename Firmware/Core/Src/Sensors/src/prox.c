/*
 * prox.c
 *
 *  Created on: May 6, 2022
 *      Author: kelvi
 */

#include "../inc/prox.h"

VL6180Dev_t vl6180Handler = 0;

void ProximitySensor_Start(I2C_HandleTypeDef* i2cHandler) {
	// For now, the handler is equal to the i2c address of the chip
	vl6180Handler = PROX_I2C_ADDRESS;
	// Workaround for not having the i2c handler for hal in vl6180 code
	RegisterI2CHandler(i2cHandler);

	// Init Chip
	int status;
	status = VL6180_InitData(vl6180Handler);
	if (status ==  CALIBRATION_WARNING) {
		// Error Handling here
	}
	status = VL6180_Prepare(vl6180Handler);
	if ( status != 0){
		//Error Handling Here
	}

	//We need to switch on the GPIO pin to change the state of the machine

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	// Need to wait according to data sheet
	HAL_Delay(0.4);
	VL6180_WaitDeviceBooted(vl6180Handler);
	//Device should be ready for commands now
	VL6180_UpscaleSetScaling(vl6180Handler, 3);
	//TODO Explore offset and cross talk calibration


}




void EnsureHandlersValid(void){
	assert_param(vl6180Handler != NULL);
	//Assert param is already in this function
	GetI2CHandler();
}


int32_t ProximitySensor_GetSingleShotMeasurement(void) {
	VL6180_RangeData_t Range;
	VL6180_RangePollMeasurement(vl6180Handler, &Range);
	if (Range.errorStatus == 0){
		//Error Handler
	}

	// Range measurement available in Range.mm
	return Range.range_mm;

}

void ProximitySensor_Stop(void){
	//This will send the proximity sensor into hardware standby mode
	VL6180_ClearAllInterrupt(vl6180Handler);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

char ProximitySensor_IsReady(void) {
	//TODO Figure out how to check if it is ready
}
