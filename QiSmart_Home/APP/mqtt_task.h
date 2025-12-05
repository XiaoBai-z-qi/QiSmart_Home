#ifndef __MQTT_TASK_H__
#define __MQTT_TASK_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"

#include "bsp_esp8266.h"


void MQTT_TaskCreate(void);
void MQTT_Task(void *arg);

#endif
