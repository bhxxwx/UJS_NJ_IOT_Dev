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
	CAN_PacketQueueHandle = xQueueCreate(5, sizeof(CAN_Packet_t));
	CAN_IT_INIT();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	xQueueSendFromISR(CAN_PacketQueueHandle, &RxMessage, NULL);

}

void cTaskCANPacketDecode(void *arg)
{

}
