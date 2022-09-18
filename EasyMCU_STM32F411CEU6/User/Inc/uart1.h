#ifndef _UART1_DEBUG_HANDLE_H_
#define _UART1_DEBUG_HANDLE_H_

/* NNK: std define */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f4xx_hal.h" 

/* NNK: User define */
#define printx_SIZE 32      //NNK: printx单次打印内存大小

/* NNK: Function extern */
extern UART_HandleTypeDef huart1;
extern void printx(char *fmt, ...);

#endif
