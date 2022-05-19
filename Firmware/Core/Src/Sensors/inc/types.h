/*
 * types.h
 *
 *  Created on: May 19, 2022
 *      Author: kelvi
 */

#ifndef SRC_SENSORS_INC_TYPES_H_
#define SRC_SENSORS_INC_TYPES_H_
// This header file holds common data structures needed throughout the sensor api


typedef enum SensorErrorType {
	OK,
	PROX_CALIBRATION_ERROR,
	PROX_INIT_ERROR,
	PROX_MEASURE_ERROR,
	PROX_NOT_RESPONDING
} SensorErrorType;


typedef void (*OnSensorErrorHandler) (enum SensorErrorType type);
struct SensorData {
	float temp;
	int32_t prox;
};




#endif /* SRC_SENSORS_INC_TYPES_H_ */
