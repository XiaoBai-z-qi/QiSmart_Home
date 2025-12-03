#ifndef __BSP_DHT11_H_
#define __BSP_DHT11_H__

#include "main.h"
#define DHT11_PIN GPIO_PIN_11
#define DHT11_PORT GPIOA

void DHT_GPIO_SET_OUTPUT(void);
void DHT_GPIO_SET_INPUT(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read(void);

#endif
