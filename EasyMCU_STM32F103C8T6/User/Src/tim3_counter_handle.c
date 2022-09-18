#include "tim3_counter_handle.h"

const char *TAG_TIM3   = "[TIM3]";

uint32_t tim3_globolTick = 0;

/* NNK: 计时器中断接收初始化函数 */
void tim3_nvic_init(void)
{
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);      //NNK: 设置中断优先级
	HAL_NVIC_EnableIRQ(TIM3_IRQn);              //NNK: 使能中断
    HAL_TIM_Base_Start_IT(&htim3);              //NNK: 开启计时器3中断
    printx("%s nvic init ok", TAG_TIM3);
}

/* NNK: 计时器中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim3)
    {
        if(tim3_globolTick == 1000)
        {
            tim3_globolTick = 0;
        }
        tim3_globolTick++;                      //NNK: 每当tim3_globolTick自增1意味着过去了1ms时间
        
        /* NNK: Easycon counter zone */
        
        if(wait_ms > 0)
        {
            wait_ms--;
        }

        timer_ms++;

        HAL_TIM_Base_Start_IT(&htim3);          //NNK: 开启计时器3中断
    }
}
