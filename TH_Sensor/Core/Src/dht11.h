/*
 * dht11.h
 *
 *  Created on: Jul 21, 2025
 *      Author: jochiho
 */

#ifndef SRC_DHT11_H_
#define SRC_DHT11_H_

#include "stm32f1xx_hal.h"

typedef struct
{
	float Temperature;
	float Humidity;
}DHT11_Data_Typedef;

void DHT11_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef DHT11_ReadData(DHT11_Date_TypeDef *data);


#endif /* SRC_DHT11_H_ */
