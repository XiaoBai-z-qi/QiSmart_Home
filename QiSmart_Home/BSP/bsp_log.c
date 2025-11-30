#include "bsp_log.h"


#define LOG_TASK_STACK_SIZE  1024     // 日志任务堆栈大小
#define LOG_MAX_LEN 128     //单条日志最大长度

static UART_HandleTypeDef *g_uart;
static QueueHandle_t logQueue;      //日志队列

static const char *level_str[] = {
    "INFO", "WARN", "DEBUG", "ERROR"
};

typedef struct{
    char buff[LOG_MAX_LEN];
}LogItem;

static void Log_Task(void *arg)
{
    LogItem item;
    while(1)
    {
        if(xQueueReceive(logQueue, &item, portMAX_DELAY) == pdTRUE) //如果队列有数据
        {
            HAL_UART_Transmit(g_uart, (uint8_t*)item.buff, strlen(item.buff), 1000); //发送日志
        }
    }
}

void Log_TaskCreate(UART_HandleTypeDef *huart)
{
    g_uart = huart;
    logQueue = xQueueCreate(10, sizeof(LogItem)); //创建日志队列

    xTaskCreate(Log_Task, "Log_Task", LOG_TASK_STACK_SIZE, NULL, 1, NULL); //创建日志任务
}

void Log_Print(const char *str)
{
    LogItem item;
    snprintf(item.buff, LOG_MAX_LEN, "%s\r\n", str);
    xQueueSend(logQueue, &item, 0); //发送日志到队列
}


void Log_PrintEx(LogLevel level, const char *file, int line, const char *fmt, ...)
{
    LogItem item;
    char text[96];

    va_list args;
    va_start(args, fmt);
    vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);

    snprintf(item.buff, LOG_MAX_LEN,"[%s][%s:%d] %s\r\n",level_str[level], file, line, text);

    xQueueSend(logQueue, &item, 0);
}