#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__
#include "main.h"

void Encoder_Init(TIM_HandleTypeDef *htim);
int32_t Encoder_GetValue(void);
#endif
