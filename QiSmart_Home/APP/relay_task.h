#ifndef __RELAY_TASK_H__
#define __RELAY_TASK_H__
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

#define RELAY0_PORT GPIOA
#define RELAY0_PIN  GPIO_PIN_8
#define RELAY1_PORT GPIOC
#define RELAY1_PIN  GPIO_PIN_9



typedef enum
{
    RELAY_OFF = 0,
    RELAY_ON = 1
}RelayCmd_t;

typedef struct
{
    uint8_t     relayId;
    RelayCmd_t  relayCmd;
}RelayMessage_t;

void Relay_TaskCreate(void);
void Relay_Task(void *arg);
static void Relay_Control(RelayMessage_t *msg);
#endif
