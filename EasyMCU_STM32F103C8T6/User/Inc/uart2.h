#ifndef _UART2_MAIN_HANDLE_H_
#define _UART2_MAIN_HANDLE_H_

/* NNK: std define */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f1xx_hal.h" 

/* NNK: User define */
#define recvBufferSize 			20     	//NNK: 主串口单次接收数据大小
#define recvPassThroughSize		7		//NNK: 主串口单次接收透传数据大小
#define sendBufferSize 			1      	//NNK: 主串口单次发送数据大小

/* NNK: SerialCon - umain cmd enum */
enum serial_cmd
{
	CMD_READY = 0xA5,
	CMD_DEBUG = 0x80,
	CMD_HELLO,
	CMD_FLASH,
	CMD_SCRIPTSTART,
	CMD_SCRIPTSTOP,
	CMD_VERSION
};

/* NNK: SerialCon - umain reply enum */
enum serial_reply
{
	REPLY_ERROR,
	REPLY_HELLO = 0x80,
	REPLY_FLASHSTART,
	REPLY_FLASHEND,
	REPLY_SCRIPTACK,
	REPLY_BUSY = 0xFE,
	REPLY_ACK
};


/* NNK: Function extern */
/* NNK: Uart(main) init */
extern void umain_nvic_init(void);
/* NNK: Uart(main) send API*/
extern int umain_send8B(uint8_t *sendbuf, uint8_t len);
extern int umain_sendCmd(uint32_t cmd);
/* NNK: Uart(main) handle */
extern int umain_handle(void);
extern void umain_cmd_handle(int cmd);
extern void umain_trans_handle(void);
extern void umain_script_handle(void);

#endif
