#include "usbd_gp_handle.h"
#include "usbd_customhid.h" 

extern USBD_HandleTypeDef hUsbDeviceFS; 



uint8_t send_buf[8] = {0x00,0x00,0x08,0x80,0x80,0x80,0x80,0x00}; 
uint8_t send_initbuf[8] = {0x00,0x00,0x08,0x80,0x80,0x80,0x80,0x00};


/* NNK: USBD原生数据发送(调试用): buf1~buf8 */
int usbd_sendRaw(uint8_t buf1, uint8_t buf2, uint8_t buf3, uint8_t buf4, uint8_t buf5, uint8_t buf6, uint8_t buf7, uint8_t buf8)
{
	uint8_t send_raw_buf[8] = {0};
	send_raw_buf[0] = buf1;
	send_raw_buf[1] = buf2;
	send_raw_buf[2] = buf3;
	send_raw_buf[3] = buf4;
	send_raw_buf[4] = buf5;
	send_raw_buf[5] = buf6;
	send_raw_buf[6] = buf7;
	send_raw_buf[7] = buf8;

	if(usbd_send(send_raw_buf, TAG_SEND_RAW_BUF) != USBD_OK)	
	{
		return USBD_SEND_BUSY;
	}
	return USBD_SEND_OK;
}

/* NNK: USBD数据初始化 */
int usbd_gp_init(void)
{
	memcpy(send_buf, send_initbuf, sizeof(send_buf));			//NNK: 需要重置send_buf内容
	if(usbd_send(send_initbuf, TAG_SEND_INIT_BUF) != USBD_OK)	
	{
		return USBD_SEND_BUSY;
	}
	return USBD_SEND_OK;
}

/* NNK: 接口函数 */ 
/* NNK: 右摇杆:rx[0,255], ry[0,255] */
int usbd_gp_rightrocker_input(uint8_t rx, uint8_t ry)
{
	if((rx <= 0) && (rx >= 255) && (ry <= 0) && (ry >= 255))	
	{
		return USBD_INPUT_OUT_OF_RANGE;
	}

	send_buf[5] = rx;
	send_buf[6] = ry;

	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}

/* NNK: 左摇杆:lx[0,255], ly[0,255] */
int usbd_gp_leftrocker_input(uint8_t lx, uint8_t ly)
{
	if((lx <= 0) && (lx >= 255) && (ly <= 0) && (ly >= 255))	
	{
		return USBD_INPUT_OUT_OF_RANGE;
	}

	send_buf[3] = lx;
	send_buf[4] = ly;

	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}

/* NNK: 2区按键: (十字键)上/下/左/右/中 */
int usbd_gp2_input(uint8_t btn)
{
	if(btn > center)	
	{
		return USBD_INPUT_OUT_OF_RANGE;
	}

	send_buf[2] = btn;

	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}

/* NNK: 1区按键: -(minus)/+(plus)/l3/r3/home/screenshot, onoff: 0/1 */
int usbd_gp1_input(uint8_t btn, uint8_t onoff)
{
	if((btn > screenshot) && (onoff != 0) && (onoff != 1))	
	{
		return USBD_INPUT_OUT_OF_RANGE;
	}
	
	send_buf[1] &= ~(1 << btn);
	send_buf[1] |= (onoff << btn);
	
	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}

/* NNK: 0区按键: y/b/a/x/l/r/zl/zr, onoff: 0/1 */
int usbd_gp0_input(uint8_t btn, uint8_t onoff)
{
	if((btn > zr) && (onoff != 0) && (onoff != 1))	
	{
		return USBD_INPUT_OUT_OF_RANGE;
	}
	
	send_buf[0] &= ~(1 << btn);
	send_buf[0] |= (onoff << btn);
	
	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}

/* NNK: serialcon专属接口: 信息全发 */
int usbd_serialcon_input_handle(uint32_t ValButton, uint8_t ValHAT, uint8_t ValLX, uint8_t ValLY, uint8_t ValRX, uint8_t ValRY)
{
	send_buf[0] = (uint8_t)(ValButton >> 0 & 0xFF);		//NNK: 取ValButton的低8位
	send_buf[1] = (uint8_t)(ValButton >> 8 & 0xFF);		//NNK: 取ValButton的高8位
	send_buf[2] = ValHAT;
	send_buf[3] = ValLX;
	send_buf[4] = ValLY;
	send_buf[5] = ValRX;
	send_buf[6] = ValRY;

	if(usbd_send(send_buf, TAG_SEND_BUF) != USBD_OK)
	{	
		return USBD_SEND_BUSY;
	}	
	return USBD_SEND_OK;
}
