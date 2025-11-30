#ifndef __BSP_LOG_H__
#define __BSP_LOG_H__
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//定义日志等级
typedef enum {
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_DEBUG,
    LOG_LVL_ERROR,
} LogLevel;

void Log_PrintEx(LogLevel level, const char *file, int line, const char *fmt, ...);

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
                     (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define LOG_INFO(fmt, ...)  \
        Log_PrintEx(LOG_LVL_INFO, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)  \
        Log_PrintEx(LOG_LVL_WARN, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
        Log_PrintEx(LOG_LVL_DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
        Log_PrintEx(LOG_LVL_ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)



void Log_TaskCreate(UART_HandleTypeDef *huart);
void Log_Print(const char *str);


#endif
