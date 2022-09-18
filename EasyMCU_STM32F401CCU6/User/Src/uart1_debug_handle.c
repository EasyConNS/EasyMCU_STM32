#include "uart1_debug_handle.h"

/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
	uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart1, temp, 1, 2);

	return ch;
}

/**
  * 函数功能: 在printf的基础上加上自动换行的功能
  * 输入参数: 任务字段字符串
  * 返 回 值: 无
  * 说    明：无
  */
void printx(char* fmt, ...)
{
  char* buf=(char*)calloc(printx_SIZE,sizeof(char));
	int ret;
  va_list args;
	
  va_start(args,fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
	
  printf(buf);
  free(buf);
  printf("\r\n");
}
