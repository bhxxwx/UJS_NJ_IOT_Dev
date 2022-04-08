/*
 * CAN.c
 *
 *  Created on: Apr 7, 2022
 *      Author: Phynix
 */

#include "CAN.h"

xQueueHandle CAN_PacketQueueHandle;
uint8_t CANSTAT_Flag;
CAN_DATA CANDATA;
TimerHandle_t CANTimerHandle;
static void TimeoutFunc(TimerHandle_t xTimer)
{
//todo send to ec20
}

void o_CAN_IT_init()
{
	CAN_PacketQueueHandle = xQueueCreate(5, sizeof(CAN_Packet_t));
	CAN_IT_INIT();
	CANTimerHandle = xTimerCreate("CAN", pdMS_TO_TICKS(1000), pdTRUE, "CAN", TimeoutFunc);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Packet_t CAN_Packet;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	CAN_Packet.ID = RxMessage.ExtId;
	CAN_Packet.DLC = RxMessage.DLC;
	memcpy(CAN_Packet.Data, RxMessage.Data, CAN_Packet.DLC);
	xQueueSendFromISR(CAN_PacketQueueHandle, &CAN_Packet, NULL);
	taskYIELD()
}

void cTaskCANPacketDecode(void *arg)
{
	for (;;)
	{
		CAN_Packet_t CAN_Packet;
		static uint8_t count = 0;
		if (CAN_PacketQueueHandle != 0)
		{
			if (xQueueReceive(CAN_PacketQueueHandle, &CAN_Packet, 500 / portTICK_RATE_MS) == pdPASS)
			{
				//todo have message
				Analysys(CAN_Packet);
				count++;
				if (CANSTAT_Flag == 0xFF)
				{
					xTimerReset(CANTimerHandle, 10);
					CANSTAT_Flag = 0;
					//todo send CANDATA to ec20
				}
			}
			else
			{
				//todo time out
			}
		}
		else
		{
			CAN_PacketQueueHandle = xQueueCreate(5, sizeof(CAN_Packet_t));
			vTaskDelay(1000 / portTICK_RATE_MS);
		}

	}
}

/**
 *
 * @param CANRX
 */
void Analysys(CAN_Packet_t CANRX)
{
	switch (CANRX.ID)
	{
		case P1ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x01;
			break;
		case P2ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x02;
			break;
		case P3ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x04;
			break;
		case P4ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x08;
			break;
		case P5ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x10;
			break;
		case P6ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x20;
			break;
		case P7ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x40;
			break;
		case P8ID:
			CANSTAT_Flag = CANSTAT_Flag | 0x80;
			break;
		default:
			return;
	}
	Analysys_DATA(CANRX);
}

/**
 *
 * @param CANRX
 */
void Analysys_DATA(CAN_Packet_t CANRX)
{
	switch (CANRX.ID)
	{
		case P1ID:
			CANDATA.HXCS = CANRX.Data[0];
			CANDATA.ZXCS = CANRX.Data[1];
			CANDATA.XLGWY = CANRX.Data[3] << 8;
			CANDATA.XLGWY += CANRX.Data[2];
			CANDATA.XLGZJ = CANRX.Data[5] << 8;
			CANDATA.XLGZJ += CANRX.Data[4];
			break;
		case P2ID:
			CANDATA.GTGD = CANRX.Data[1] << 8;
			CANDATA.GTGD += CANRX.Data[0];
			CANDATA.BHLGD = CANRX.Data[3] << 8;
			CANDATA.BHLGD += CANRX.Data[2];
			CANDATA.XGD = CANRX.Data[5] << 8;
			CANDATA.XGD += CANRX.Data[4];
			break;
		case P3ID:
			CANDATA.DPZQ = CANRX.Data[1] << 8;
			CANDATA.DPZQ += CANRX.Data[0];
			CANDATA.DPYQ = CANRX.Data[3] << 8;
			CANDATA.DPYQ += CANRX.Data[2];
			CANDATA.DPZH = CANRX.Data[5] << 8;
			CANDATA.DPZH += CANRX.Data[4];
			CANDATA.DPYH = CANRX.Data[7] << 8;
			CANDATA.DPYH += CANRX.Data[6];
			break;
		case P4ID:
			CANDATA.QJSD = CANRX.Data[3] << 8;
			CANDATA.QJSD += CANRX.Data[2];
			CANDATA.LZLL = CANRX.Data[5] << 8;
			CANDATA.LZLL += CANRX.Data[4];
			break;
		case P5ID:
			CANDATA.ZGDZS = CANRX.Data[1] << 8;
			CANDATA.ZGDZS += CANRX.Data[0];
			CANDATA.BHLZS = CANRX.Data[3] << 8;
			CANDATA.BHLZS += CANRX.Data[2];
			CANDATA.SSCZS = CANRX.Data[5] << 8;
			CANDATA.SSCZS += CANRX.Data[4];
			CANDATA.TLGT = CANRX.Data[7] << 8;
			CANDATA.TLGT += CANRX.Data[6];
			break;
		case P6ID:
			CANDATA.FJZS = CANRX.Data[1] << 8;
			CANDATA.FJZS += CANRX.Data[0];
			CANDATA.SLJLZS = CANRX.Data[3] << 8;
			CANDATA.SLJLZS += CANRX.Data[2];
			CANDATA.ZYJLZS = CANRX.Data[5] << 8;
			CANDATA.ZYJLZS += CANRX.Data[4];
			break;
		case P7ID:
			CANDATA.QXSS = CANRX.Data[1] << 8;
			CANDATA.QXSS += CANRX.Data[0];
			CANDATA.JDSS = CANRX.Data[3] << 8;
			CANDATA.JDSS += CANRX.Data[2];
			CANDATA.TLJX = CANRX.Data[4];
			CANDATA.DLBKD += CANRX.Data[5];
			CANDATA.JFKKD += CANRX.Data[6];
			CANDATA.YLKSD += CANRX.Data[7];
			break;
		case P8ID:
			CANDATA.GFKD = CANRX.Data[4] << 8;
			CANDATA.GFKD += CANRX.Data[3];
			CANDATA.HZL = CANRX.Data[5];
			CANDATA.PSL += CANRX.Data[6];
			break;
		default:
			break;
	}
}
