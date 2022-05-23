/*
 * prox.c
 *
 *  Created on: May 6, 2022
 *      Author: kelvi
 */

#include "../inc/prox.h"

VL6180Dev_t vl6180Handler = (PROX_I2C_ADDRESS & 0x7f) << 1;

/*
 * @description
 * Starts the proximity sensor
 *
 * @arguments
 * @i2cHandler - The I2C Handler
 *
 * @returns SensorerrorType
 */

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


/*
 * @description
 * Internal function to ensure that the various handlers the api uses are still valid
 */

void EnsureHandlersValid(void){
	assert_param(vl6180Handler != NULL);
	//Assert param is already in this function
	GetI2CHandler();
}

/*
 * @description
 * Gets a distance measurement once at a time
 * @arguments
 * @result - A pointer to the integer variable holding the result of the operation in millimetres
 */
SensorErrorType ProximitySensor_GetSingleShotMeasurement(int32_t* result) {
	VL6180_RangeData_t Range;
	VL6180_RangePollMeasurement(vl6180Handler, &Range);
	if (Range.errorStatus != 0){
		//Error Handler
		//return PROX_MEASURE_ERROR;
	}

	// Range measurement available in Range.mm
	(*result) = Range.range_mm;
	return OK;

}
/*
 * @description
 * Places the proximity sensor in standby mode
 * @returns
 * SensorErrorType
 */
SensorErrorType ProximitySensor_Stop(void){
	//This will send the proximity sensor into hardware standby mode
	VL6180_ClearAllInterrupt(vl6180Handler);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	return OK;
}

/*
 * @description
 * Checks whether the proximity sensor is ready and responding
 * @returns
 * SensorErrorType
 */
SensorErrorType ProximitySensor_IsReady(void) {
	I2C_HandleTypeDef* handler = GetI2CHandler();

	if (HAL_I2C_IsDeviceReady(handler, (0x29 & 0x7f) << 1, 3, 1) != 0){
		return PROX_NOT_RESPONDING;
	}
	return OK;
}

/*
 * @description
 * Starts the proximity sensor in interrupt mode. User is responsible for setting up the GPIO interrupt in hardware
 *
 * @arguments
 * @i2cHandler - The I2C Handler
 * @freq - The frequency at which interrupts are called
 */

SensorErrorType ProximitySensor_Start_IT(I2C_HandleTypeDef* i2cHandler, int freq){
	SensorErrorType result = ProximitySensor_Start(i2cHandler);
	if (result != OK){
		return result;
	}
	if (VL6180_FilterSetState(vl6180Handler, 0)){
		//Returns 1 if error
		return PROX_INIT_ERROR;
	}
	if (VL6180_RangeSetInterMeasPeriod(vl6180Handler, 1/freq)){
			//Returns 1 if error
			return PROX_INIT_ERROR;
	}
	if (VL6180_SetupGPIO1(vl6180Handler,  GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH)){
			//Returns 1 if error
			return PROX_INIT_ERROR;
	}

	return OK;



}
/*
 * @description
 * Function to be called once the interrupt detection has been set up
 */
SensorErrorType ProximitySensor_OnInterruptStarted(void){
	VL6180_ClearAllInterrupt(vl6180Handler);
	VL6180_RangeStartContinuousMode(vl6180Handler);
	return OK;
}

/*
 * @description
 * Called when the interrupt fires. Will output the measurement results
 *
 */

SensorErrorType ProximitySensor_OnInterruptDetected(int32_t* result){
	VL6180_RangeData_t Range;
	if (VL6180_RangeGetMeasurement(vl6180Handler, &Range)){
		return PROX_MEASURE_ERROR;
	}
	if (Range.errorStatus == 0){
		return PROX_MEASURE_ERROR;
	}
	(*result) = Range.range_mm;
	VL6180_RangeClearInterrupt(vl6180Handler);

}
