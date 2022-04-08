#include "UserConfig.h"
#include "stm32f10x.h"
#include <stdio.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
//#include<pthread.h>

int main()
{
	DBGMCU_Config(DBGMCU_IWDG_STOP, ENABLE); //DEBUG时看门狗关闭
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	CAN_IT_INIT();

	vTaskStartScheduler();
	return 0;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{

}
