/*
 * EC20.c
 *
 *  Created on: Apr 8, 2022
 *      Author: Phynix
 */

#include "EC20.h"
#define CreateEventMapEle(curST,nextST,transEV) {.curState=curST,.nextState=nextST,.event=transEV}


const EC_EventMap_t EC_EventMap[] = {
CreateEventMapEle(PowerOn, SystemReset, EC_OK),
CreateEventMapEle(SystemReset, CheckRFlevel, EC_OK),
CreateEventMapEle(CheckRFlevel, SystemReset, EC_RFlow),
CreateEventMapEle(CheckRFlevel, CheckNetworkReg, EC_OK) };


void EC20_init()
{
	usart_1_init(115200);
}

void EC_StateMachineInit(EC_StateMachine_t *pEC_Machine, EC_EventMap_t *pEventMap,
        EC_ActionMap_t *pActionMap)
{
	pEC_Machine->curState = PowerOn;
	pEC_Machine->nextState = SystemReset;
	pEC_Machine->pActionMap = pActionMap;
	pEC_Machine->pEventMap = pEventMap;
}
