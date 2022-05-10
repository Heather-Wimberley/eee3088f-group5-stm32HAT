#include "../inc/temp.h"


ADC_HandleTypeDef *ADCState;
char SensorState = 0;

//Starts the temperature sensor
// hadc is the handle pointer to the global adc handler
void TempSensor_Start(ADC_HandleTypeDef *hadc){
	//Switch on the GPIO Pin controlling the sensor
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);


	//Set up the ADC
	HAL_ADCEx_Calibration_Start(hadc);
	ADCState = hadc;




}

// Get the current measurement from the temperature sensor
// result is a pointer to a double which will hold the result
// maxTime is the maximum amount of time you can wait before a result needs to be returned
// Can be dictated by the sampling rate
float TempSensor_GetMeasurement(int maxTime) {
	assert_param(TempSensor_HasStarted());
	HAL_ADC_Start(ADCState);
	HAL_ADC_PollForConversion(ADCState, maxTime);

	float result = HAL_ADC_GetValue(ADCState);

	//Convert from steps to voltage to temperature
	// reusing result variable to store voltage
	// voltage is the result multiplied by the voltage value of 1 level of the ADC
	result = result * (3.3/4096);
	// change the voltage result to temperature according to MCP9700 datasheet
	result =  (100 * result) - 50;
	return result;
}

char TempSensor_HasStarted(void){
	EnsureHandleValid();
	return SensorState;
}

void EnsureHandleValid(void){
	assert_param(ADCState != NULL);
}

void TempSensor_Stop(void) {
	// Switching off the GPIO Pin is enough to switch off the sensor
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	//Problematic, whether switching off this adc might switch it off for the end user
	HAL_ADC_Stop(ADCState);
}
