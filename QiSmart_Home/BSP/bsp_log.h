#ifndef __BSP_LOG_H__
#define __BSP_LOG_H__
#include "usart.h"


void Log_TaskCreate(UART_HandleTypeDef *huart);
void Log_Print(const char *str);


#endif
