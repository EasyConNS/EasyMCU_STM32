#ifndef _TIM3_COUNTER_HANDLE_H_
#define _TIM3_COUNTER_HANDLE_H_

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f4xx_hal.h" 

/* NNK: User define */
#ifdef STM32F103xB
#define TIM_Freq                72
#define TIM_SingleTickTime      one_ms
#elif STM32F103x6
#define TIM_Freq                72
#define TIM_SingleTickTime      one_ms
#elif STM32F401xC
#define TIM_Freq                60
#define TIM_SingleTickTime      one_ms
#endif

/* NNK: tim3 - single tick enum */
enum single_tick_time
{
	zero_point_one_ms  = 1,     //NNK: 单次计时器3中断时间 = 0.1ms
    one_ms             = 10,    //NNK: 单次计时器3中断时间 = 1ms
    ten_ms             = 100,   //NNK: 单次计时器3中断时间 = 10ms
    hundred_ms         = 1000   //NNK: 单次计时器3中断时间 = 100ms
};

/* NNK: Function extern */
/* NNK: tim3 init */
extern void tim3_nvic_init(void);

#endif
