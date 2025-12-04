#ifndef __BSP_ESP8266_H__
#define __BSP_ESP8266_H__
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cmsis_os.h"

#define AT_CMD_TEST "AT\r\n"
#define AT_SET_STA     "AT+CWMODE=1\r\n"
#define AT_CWJAP  "AT+CWJAP=\"%s\",\"%s\"\r\n"

#define AT_ACK_OK "OK"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void ESP_InitStreamBuffer(void);
bool ESP_Init(void);
bool ESP_WIFIConnect(uint16_t timeout);





bool ESP_SendCmd(char *cmd, char *ack, uint32_t timeout);
#endif
