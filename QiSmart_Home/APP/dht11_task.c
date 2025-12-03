#include "dht11_task.h"

extern uint8_t Data[5]; 
uint8_t humidity, temperature;


TaskHandle_t  dht11TaskHandle;
void DHT11_TaskCreate(void)
{
    xTaskCreate(DHT11_Task, "DHT11_Task", 128, NULL, osPriorityNormal2, &dht11TaskHandle);
}


void DHT11_Task(void *arg)
{
    while(1)
    {
        if(DHT_Read())
        {
            humidity = Data[0];
            temperature = Data[2];
        }
        vTaskDelay(pdMS_TO_TICKS(1000));

    }

}
