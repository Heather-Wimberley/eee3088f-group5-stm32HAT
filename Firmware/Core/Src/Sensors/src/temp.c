#include "../inc/temp.h"


ADC_HandleTypeDef *ADCState;
char SensorState = 0;

/* @description
 * Starts the temperature sensor
 * @arguments
 * hadc is the handle pointer to the global adc handler
 *
 */
SensorErrorType TempSensor_Start(ADC_HandleTypeDef *hadc){
	//Switch on the GPIO Pin controlling the sensor
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);



	//Set up the ADC
	HAL_ADCEx_Calibration_Start(hadc);
	ADCState = hadc;
	SensorState = 1;
	return OK;


}

/*
 * @description
 * Get the current measurement from the temperature sensor
 * @arguments
 * result - is a pointer to a double which will hold the result
 * maxTime - is the maximum amount of time you can wait before a result needs to be returned
 * Can be dictated by the sampling rate
 */
SensorErrorType TempSensor_GetMeasurement(int maxTime, float* result) {
	assert_param(TempSensor_HasStarted());
	HAL_ADC_Start(ADCState);
	HAL_ADC_PollForConversion(ADCState, maxTime);

	float data = HAL_ADC_GetValue(ADCState);
	float voltage = data * (3.3/4096);

	//Convert from steps to voltage to temperature
	// reusing result variable to store voltage
	// voltage is the result multiplied by the voltage value of 1 level of the ADC

	// change the voltage result to temperature according to MCP9700 datasheet

	(*result) = (75*voltage) - 50;
	return OK;

}

/*
 * @description
 * Checks if the temperature has started
 * @returns
 * SensorErrorType
 */
SensorErrorType TempSensor_HasStarted(void){
	EnsureHandleValid();
	if (SensorState){
		return OK;
	}
	return TEMP_NOT_STARTED;
}

void EnsureHandleValid(void){
	assert_param(ADCState != NULL);
}

/*
 * @description
 * Stop the temperature sensor
 */
SensorErrorType TempSensor_Stop(void) {
	// Switching off the GPIO Pin is enough to switch off the sensor
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	//Problematic, whether switching off this adc might switch it off for the end user
	HAL_ADC_Stop(ADCState);
	SensorState = 0;
	return OK;
}
