#include "ledb_handle.h"

/* NNK: Led开灯函数 */
void ledb_on(void)
{
	HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_RESET);
}

/* NNK: Led关灯函数 */
void ledb_off(void)
{
	HAL_GPIO_WritePin(LEDB_GPIO_Port, LEDB_Pin, GPIO_PIN_SET);
}

/* NNK: Led测试函数 */
void ledb_test(void)
{
	for(int i=0; i<3; i++)
	{
		ledb_on();
		HAL_Delay(100);
		ledb_off();
		HAL_Delay(100);
	}
}
