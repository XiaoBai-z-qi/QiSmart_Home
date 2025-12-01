#ifndef __SENSOR_TASK_H__
#define __SENSOR_TASK_H__
#include "FreeRTOS.h"
#include "bsp_encoder.h"
#include "queue.h"
#include "cmsis_os.h"

extern QueueHandle_t SensorQueueHandle;

void Sensor_Init(ADC_HandleTypeDef *hadc);
void Sensor_TaskCreate(ADC_HandleTypeDef *hadc);
void Sensor_Task(void *arg);
#endif
