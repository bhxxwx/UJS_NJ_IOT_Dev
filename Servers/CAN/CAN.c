/*
 * CAN.c
 *
 *  Created on: Apr 7, 2022
 *      Author: Phynix
 */

#include "CAN.h"


xQueueHandle CAN_PacketQueueHandle;

void o_CAN_IT_init()
{
	CAN_PacketQueueHandle = xQueueCreate(10, sizeof(CAN_Packet_t));
}
