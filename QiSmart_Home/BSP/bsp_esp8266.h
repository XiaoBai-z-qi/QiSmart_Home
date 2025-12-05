#ifndef __BSP_ESP8266_H__
#define __BSP_ESP8266_H__
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "portmacro.h" 

#define ESP8266_WIFI_CONFIG           "AT+CWJAP=\"Xiaomi8266\",\"123456789\"\r\n"
#define MQTT_CONN  										"AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n"
#define MQTT_USERCFG       						"AT+MQTTUSERCFG=0,1,\"device-001\",\"Ez68Tg4NJD\",\"version=2018-10-31&res=products%2FEz68Tg4NJD%2Fdevices%2Fdevice-001&et=2080346120&method=md5&sign=fXj3%2B8EN0dPY%2BzL0NFPEqg%3D%3D\",0,0,\"\"\r\n"
#define ESP8266_DHTP                	"AT+CWDHCP=1,1\r\n"




#define CMD_AT "AT\r\n"
#define CMD_CWMOED     "AT+CWMODE=1\r\n"
#define CMD_RST       "AT+RST\r\n"
#define CMD_CWJAP  "AT+CWJAP=\"%s\",\"%s\"\r\n"

#define ACK_OK "OK"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void ESP_InitStreamBuffer(void);
bool ESP_Init(void);
bool MQTT_SendDate(const char *str, uint8_t date);
// bool ESP_WIFIConnect(uint16_t timeout);





bool ESP_SendCmd(char *cmd, char *ack, uint32_t timeout);
bool MQTT_SendTempHumi(int16_t temp, uint16_t humi);
#endif
