#ifndef __DHT11_TASK_H__
#define __DHT11_TASK_H__
#include "FreeRTOS.h"
#include "bsp_dht11.h"
#include "queue.h"
#include "cmsis_os.h"


void DHT11_TaskCreate(void);
void DHT11_Task(void *arg);
#endif
