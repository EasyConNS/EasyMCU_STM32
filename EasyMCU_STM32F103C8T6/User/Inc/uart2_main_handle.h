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

/* NNK: SerialCon - Joystick button enum */
enum JoystickButtons_t
{
    SWITCH_Y        = 0x01,
	SWITCH_B        = 0x02,
	SWITCH_A        = 0x04,
	SWITCH_X        = 0x08,
	SWITCH_L        = 0x10,
	SWITCH_R        = 0x20,
	SWITCH_ZL       = 0x40,
	SWITCH_ZR       = 0x80,
	SWITCH_MINUS    = 0x100,
	SWITCH_PLUS     = 0x200,
	SWITCH_LCLICK   = 0x400,
	SWITCH_RCLICK   = 0x800,
	SWITCH_HOME     = 0x1000,
	SWITCH_CAPTURE  = 0x2000
};

/* NNK: SerialCon - Hat button enum */
enum HatButtons_t
{
    HAT_TOP             = 0x00,
    HAT_TOP_RIGHT       = 0x01,
    HAT_RIGHT           = 0x02,
    HAT_BOTTOM_RIGHT    = 0x03,
    HAT_BOTTOM          = 0x04,
    HAT_BOTTOM_LEFT     = 0x05,
    HAT_LEFT            = 0x06,
    HAT_TOP_LEFT        = 0x07,
    HAT_CENTER          = 0x08
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
