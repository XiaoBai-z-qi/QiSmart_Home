#include "bsp_esp8266.h"
#include "bsp_log.h"
#include "m_config.h"
StreamBufferHandle_t espRxStreamBuffer = NULL;
uint8_t rx_char;
const  char* pubtopic="$sys/Ez68Tg4NJD/device-001/thing/property/post";
void ESP_InitStreamBuffer(void)
{
	  HAL_UART_Receive_IT(ESP8266_UART, &rx_char, 1);
    //创建流缓冲区，256字节 触发level = 1 表示有1个字节就能读取
    espRxStreamBuffer = xStreamBufferCreate(384, 1);
}

bool ESP_SendCmd(char *cmd, char *ack, uint32_t timeout)
{
    //检查流缓冲区是否已初始化
    if(espRxStreamBuffer == NULL)   return false;

    //清空流缓冲区
    xStreamBufferReset(espRxStreamBuffer);

    //发送命令
    HAL_UART_Transmit_IT(ESP8266_UART, (uint8_t*)cmd, strlen(cmd));

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
    if(huart == ESP8266_UART && espRxStreamBuffer)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xStreamBufferSendFromISR(espRxStreamBuffer, &rx_char, 1, &xHigherPriorityTaskWoken);
        HAL_UART_Receive_IT(ESP8266_UART, &rx_char, 1);   // 重新启动中断
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

bool ESP_Init(void)
{
    if(!ESP_SendCmd(CMD_AT, ACK_OK, 200))                   return false;
		vTaskDelay(pdMS_TO_TICKS(500));

    if(!ESP_SendCmd(CMD_RST, "", 200))                      return false;
		vTaskDelay(pdMS_TO_TICKS(500));

    if(!ESP_SendCmd(CMD_CWMOED, ACK_OK, 200))               return false;
		vTaskDelay(pdMS_TO_TICKS(500));

	  if(!ESP_SendCmd(ESP8266_DHTP, ACK_OK, 200))               return false;
		vTaskDelay(pdMS_TO_TICKS(500));

    if(!ESP_SendCmd(ESP8266_WIFI_CONFIG, ACK_OK, 2000))     return false;
		vTaskDelay(pdMS_TO_TICKS(500));

    if(!ESP_SendCmd(MQTT_USERCFG, ACK_OK, 2000))               return false;

		vTaskDelay(pdMS_TO_TICKS(500));
    if(!ESP_SendCmd(MQTT_CONN, ACK_OK, 8000))            return false;

    return true;
}

bool MQTT_SendDate(const char *str, uint8_t date)
{
	char cmdbuf[384] = {0};
  	sprintf(cmdbuf, "AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%d}\\}}\",0,0\r\n",pubtopic, str, date);
	return ESP_SendCmd(cmdbuf, ACK_OK, 2000);
}

bool MQTT_SendTempHumi(int16_t temp, uint16_t humi)
{
  char cmdbuf[384] = {0};
  sprintf(cmdbuf, "AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"temperature\\\":{\\\"value\\\":%d}\\,\\\"humidity\\\":{\\\"value\\\":%d}\\}}\",0,0\r\n",pubtopic, temp, humi);
  return ESP_SendCmd(cmdbuf, ACK_OK, 2000);
}




