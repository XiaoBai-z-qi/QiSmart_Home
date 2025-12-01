#include "bsp_encoder.h"

static int16_t last_count = 0;
static int16_t encoder_value = 0;

TIM_HandleTypeDef etim;
void Encoder_Init(TIM_HandleTypeDef *htim)
{
    etim = *htim;
    HAL_TIM_Encoder_Start(&etim, TIM_CHANNEL_ALL);
    last_count = HAL_TIM_ReadCapturedValue(&etim, TIM_CHANNEL_ALL);
		__HAL_TIM_SET_COUNTER(&etim, 0);
}

int32_t Encoder_GetValue(void)
{
    int16_t now = __HAL_TIM_GET_COUNTER(&etim); // 当前计数值
    int16_t diff = now - last_count;             // 和上一次比较

    // 处理计数值从65535回到0（或0到65535）时造成的跳变
    if(diff > 20000) diff -= 65536;
    else if(diff < -20000) diff += 65536;

    encoder_value += diff;  // 增量累加
    last_count = now;       // 更新为前次值等待下次比较

    return encoder_value;   // 返回累计旋转值
}