#include "mqtt_task.h"
#include <stdbool.h>

extern uint8_t humidity, temperature;

TaskHandle_t  MQTTHandle = NULL;

void MQTT_TaskCreate(void)
{
    xTaskCreate(MQTT_Task, "MQTT_Task", 1024, NULL, 1, &MQTTHandle);
}



void MQTT_Task(void *arg)
{
    uint8_t mqtt_flag = 0;
    if(ESP_Init() == true)
        mqtt_flag = 1;
    else
        mqtt_flag = 0;

    while(1)
    {
        if(mqtt_flag == 1)
        {

            MQTT_SendTempHumi(temperature,humidity);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));

    }
}


