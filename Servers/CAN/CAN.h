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
#include "timers.h"

#define P1ID 0x18FF2111
#define P2ID 0x18FF2313
#define P3ID 0x18FF2413
#define P4ID 0x0CFF2715
#define P5ID 0x18FF2815
#define P6ID 0x18FF2915
#define P7ID 0x18FF2515
#define P8ID 0x0CFF2615
#define P9ID 0x18FF2817


typedef struct
{
	uint32_t ID;
	uint8_t Data[8];
	uint8_t DLC;

} CAN_Packet_t;

typedef struct
{
	uint16_t HXCS;
	uint16_t ZXCS;
	uint16_t XLGWY;
	uint16_t XLGZJ;

	uint16_t GTGD;
	uint16_t BHLGD;
	uint16_t XGD;

	uint16_t DPZQ;
	uint16_t DPYQ;
	uint16_t DPZH;
	uint16_t DPYH;

	uint16_t QJSD;
	uint16_t LZLL;

	uint16_t ZGDZS;
	uint16_t BHLZS;
	uint16_t SSCZS;
	uint16_t TLGT;

	uint16_t FJZS;
	uint16_t SLJLZS;
	uint16_t ZYJLZS;

	uint16_t QXSS;
	uint16_t JDSS;
	uint16_t TLJX;
	uint16_t DLBKD;
	uint16_t JFKKD;
	uint16_t YLKSD;

	uint16_t GFKD;
	uint16_t HZL;
	uint16_t PSL;

} CAN_DATA;


void o_CAN_IT_init();
void Analysys(CAN_Packet_t CANRX);
void Analysys_DATA(CAN_Packet_t CANRX);
static void TimeoutFunc(TimerHandle_t xTimer);
#endif /* CAN_CAN_H_ */
