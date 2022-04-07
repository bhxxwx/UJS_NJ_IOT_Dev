/*
 * CAN.h
 *
 *  Created on: Apr 7, 2022
 *      Author: Phynix
 */

#ifndef CAN_CAN_H_
#define CAN_CAN_H_


#include "stm32f10x.h"
#include "UserConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct
{
	uint16_t ID;
	uint8_t Data[8];
	uint8_t DLC;

} CAN_Packet_t;


void o_CAN_IT_init();


#endif /* CAN_CAN_H_ */
