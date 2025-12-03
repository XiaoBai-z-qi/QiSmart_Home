#include "sys_delay.h"

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 使能 DWT
    DWT->CYCCNT = 0;                                // 复位计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // 启动计数
}

void Delay_us(uint16_t us)
{
    uint32_t us_count = us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < us_count);
}

