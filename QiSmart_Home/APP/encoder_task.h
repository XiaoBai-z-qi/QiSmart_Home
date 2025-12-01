#ifndef __ENCODER_TASK_H__
#define __ENCODER_TASK_H__

#include "FreeRTOS.h"
#include "bsp_encoder.h"
#include "queue.h"
#include "cmsis_os.h"

extern QueueHandle_t encoderQueueHandle;
void Encoder_TaskCreate(TIM_HandleTypeDef *htim);
static void Encoder_Task(void *arg);
#endif
