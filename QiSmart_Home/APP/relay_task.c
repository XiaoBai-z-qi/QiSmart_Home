#include "relay_task.h"

TaskHandle_t  RelayHandle;
QueueHandle_t RelayQueue;


void Relay_TaskCreate(void)
{
    RelayQueue = xQueueCreate(10, sizeof(RelayMessage_t));
    xTaskCreate(Relay_Task, "Relay_Task", 128, NULL, osPriorityNormal2, &RelayHandle);
}


void Relay_Task(void *arg)
{
    RelayMessage_t msg;
    while(1)
    {
        // if(xQueueReceive(RelayQueue, &msg, portMAX_DELAY) == pdTRUE)
        // {
        //     Relay_Control(&msg);
        // }
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void Relay_Control(RelayMessage_t *msg)
{
    if(msg->relayId == 0)
    {
        if(msg->relayCmd == RELAY_OFF)
            HAL_GPIO_WritePin(RELAY0_PORT, RELAY0_PIN, GPIO_PIN_RESET);
        else if(msg->relayCmd == RELAY_ON)
            HAL_GPIO_WritePin(RELAY0_PORT, RELAY0_PIN, GPIO_PIN_SET);
    }
    else if(msg->relayId == 1)
    {
        if(msg->relayCmd == RELAY_OFF)
            HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_RESET);
        else if(msg->relayCmd == RELAY_ON)
            HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_SET);
    }
}

