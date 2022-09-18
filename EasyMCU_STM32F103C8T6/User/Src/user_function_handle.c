#include "user_function_handle.h"

const char *TAG_USER = "[USER]";

/* NNK: 假随机延时函数(delay_ms_ps & delay_second_ps)用变量 */
static uint8_t randVal = 0;             //NNK: 记录上一次假随机值

/* NNK: 第一弹接口/函数 */
/* NNK: 延时函数，单位毫秒 */
int delay_ms(int ms)
{
    HAL_Delay(ms);
    return ms;
}

/* NNK: 延时函数，单位秒 */
int delay_second(int second)
{
    int ms = second * 1000;
    HAL_Delay(ms);
    return ms;
}

/* NNK: 假随机延时函数，单位毫秒(PS -> Pseudorandom) */
int delay_ms_ps(int ms)
{
    int outVal = ms + (randVal * 3 % 10) * 10;
    randVal++;
    HAL_Delay(outVal);
    return outVal;
}

/* NNK: 假随机延时函数，单位秒(PS -> Pseudorandom) */
int delay_second_ps(int second)
{
    int outVal = second * 1000 + (randVal * 3 % 10) * 10;
    randVal++;
    HAL_Delay(outVal);
    return outVal;
}

/* NNK: 第二弹接口/函数 */
/* NNK: 结束函数 */
void end_loop(void)
{
    while(1)
    {
        delay_ms(1000);
    }
}
