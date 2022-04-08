/*
 * Log.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Phynix
 */

#ifndef LOG_LOG_H_
#define LOG_LOG_H_
#include "stm32f10x.h"
#include "UserConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void Log_Init();

void SendLog(const char *str);


#endif /* LOG_LOG_H_ */
