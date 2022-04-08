/*
 * EC20.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Phynix
 */

#ifndef EC20_EC20_H_
#define EC20_EC20_H_


#include "stm32f10x.h"
#include "UserConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

typedef void (*EC_STATE_ACTION)(void);

typedef enum
{
	PowerOn,
	SystemReset,
	CheckRFlevel,
	CheckNetworkReg,
	ConfigMQTT,
	OpenMQTT,
	ConnectServer,
	SubTopic,
	PubMessage
} EC_states_t;

typedef enum
{
	EC_OK, EC_ERROR, EC_LostConnect, EC_RFlow, EC_EVENT_MAX

} EC_EVENT_t;

typedef struct
{
	EC_states_t stateID;
	EC_STATE_ACTION action;
	uint8_t repeatTimes;
} EC_ActionMap_t;

typedef struct
{
	EC_states_t curState;
	EC_EVENT_t event;
	EC_states_t nextState;

} EC_EventMap_t;

typedef struct EC_StateMachine
{
	EC_states_t curState;
	EC_states_t nextState;
	EC_ActionMap_t *pActionMap;
	EC_EventMap_t *pEventMap;
} EC_StateMachine_t;

void EC20_init();

#endif /* EC20_EC20_H_ */
