/*
 * vl6180_plaform.c
 *
 *  Created on: May 6, 2022
 *      Author: kelvi
 */

#include "stm32f0xx_hal.h"

#include "../inc/vl6180_platform.h"

// This file contains some functions that need implementations from the vl6180_plaform header file


void VL6180_PollDelay(VL6180Dev_t dev) {
  // Delay by 1 ms according to the specifications
	HAL_Delay(1);
}
