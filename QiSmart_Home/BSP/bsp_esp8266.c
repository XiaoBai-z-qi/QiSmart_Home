#include "bsp_esp8266.h"
#include "m_config.h"
StreamBufferHandle_t espRxStreamBuffer = NULL;
uint8_t rx_char;

void ESP_InitStreamBuffer(void)
{
	HAL_UART_Receive_IT(ESP8266_UART, &rx_char, 1);
    //创建流缓冲区，256字节 触发level = 1 表示有1个字节就能读取
    espRxStreamBuffer = xStreamBufferCreate(256, 1);
}

bool ESP_SendCmd(char *cmd, char *ack, uint32_t timeout)
{
    //检查流缓冲区是否已初始化
    if(espRxStreamBuffer == NULL)   return false;

    //清空流缓冲区
    xStreamBufferReset(espRxStreamBuffer);

    //发送命令
    HAL_UART_Transmit(ESP8266_UART, (uint8_t*)cmd, strlen(cmd), 100);

    TickType_t start_tick = xTaskGetTickCount();
    TickType_t max_wait = pdMS_TO_TICKS(timeout);

    char recv_buff[256] = {0};  //临时缓存
    size_t recv_len = 0;        //已接收长度

    while((xTaskGetTickCount() - start_tick) < max_wait)
    {
        //查看流缓冲区里有多少新数据
        size_t available = xStreamBufferBytesAvailable(espRxStreamBuffer);
        if(available > 0)
        {
            //需要读取多少
            size_t to_read = MIN(available, sizeof(recv_buff) - recv_len);
            if(to_read > 0)
            {
                //从streambuffer读取数据
                size_t read = xStreamBufferReceive(espRxStreamBuffer, (void*)(recv_buff + recv_len), to_read, 0);
                recv_len += read;
                recv_buff[recv_len] = '\0';

                if(strstr(recv_buff, ack)!= NULL)
                    return true;

                if(recv_len >= sizeof(recv_buff) - 1)
                {
                    recv_len = 0;
                    memset(recv_buff, 0, sizeof(recv_buff));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    return false;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == ESP8266_UART)
    {
        if(espRxStreamBuffer!= NULL)
        {
            xStreamBufferSendFromISR(espRxStreamBuffer, &rx_char, 1, NULL);
        }
        
        HAL_UART_Receive_IT(ESP8266_UART, &rx_char, 1);
    }
}

bool ESP_Init(void)
{
    if(!ESP_SendCmd(AT_CMD_TEST, AT_ACK_OK, 200))   return false;
    if(!ESP_SendCmd(AT_SET_STA, AT_ACK_OK, 200))    return false;
    return true;
}

bool ESP_WIFIConnect(uint16_t timeout)
{
    char cmd[100] = {0};
    snprintf(cmd, sizeof(cmd), AT_CWJAP, WIFI_SSID, WIFI_PASSWD);
    return ESP_SendCmd(cmd, AT_ACK_OK, timeout);
}