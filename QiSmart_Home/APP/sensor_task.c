#include "sensor_task.h"
uint16_t adc_buf[1];
ADC_HandleTypeDef madc;

TaskHandle_t SensorHandle;
QueueHandle_t SensorQueueHandle;

void Sensor_TaskCreate(ADC_HandleTypeDef *hadc)
{
    SensorQueueHandle = xQueueCreate(4, sizeof(uint16_t));
    Sensor_Init(hadc);
    xTaskCreate(Sensor_Task, "Sensor_Task", 64, NULL, osPriorityNormal1, &SensorHandle);
}

void Sensor_Task(void *arg)
{
    while(1)
    {
        xQueueSend(SensorQueueHandle, &adc_buf[0], 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Sensor_Init(ADC_HandleTypeDef *hadc)
{
    madc = *hadc;
    HAL_ADC_Start_DMA(&madc, (uint32_t*)adc_buf, 1);    //启动ADC的DMA转换
}

