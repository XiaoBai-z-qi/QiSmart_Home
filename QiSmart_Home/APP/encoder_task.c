#include "encoder_task.h"


TaskHandle_t encoderTaskHandle;
QueueHandle_t encoderQueueHandle;

void Encoder_TaskCreate(TIM_HandleTypeDef *htim)
{
    //创建队列(最多存4个)
    encoderQueueHandle = xQueueCreate(4, sizeof(int16_t));
    Encoder_Init(htim);
    xTaskCreate(Encoder_Task, "Encoder_Task", 128, NULL, osPriorityNormal2, &encoderTaskHandle);
}

static void Encoder_Task(void *arg)
{
    while(1)
    {
        int16_t val = Encoder_GetValue();
        //非阻塞发送数据到队列
        xQueueSend(encoderQueueHandle, &val, 0);
        vTaskDelay(pdMS_TO_TICKS(20));  //以50Hz的频率采集数据
    }
}
