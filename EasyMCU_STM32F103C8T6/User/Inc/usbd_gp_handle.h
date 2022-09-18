#ifndef _USBD_GP_HANDLE_H_
#define _USBD_GP_HANDLE_H_

/* NNK: std define */
#include <string.h>

/* NNK: User include */
#include "main.h"
#include "uart2_main_handle.h"

/* NNK: enum for send err code */
enum errcode
{   
    USBD_INPUT_OUT_OF_RANGE = -1,
    USBD_SEND_BUSY,
    USBD_SEND_OK
};

/* NNK: enum for btn zone 1 */
enum gp2
{   
    up, upper_right, right, lower_right, down, lower_left, left, upper_left, center
};

/* NNK: enum for btn zone 1 */
enum gp1
{   
    minus, plus, l3, r3, home, screenshot
};

/* NNK: enum for btn zone 0 */
enum gp0
{   
    y, b, a, x, l, r, zl, zr
};

/* NNK: Function extern */
/* NNK: USB HID init */
extern int usbd_gp_init(void);
/* NNK: USB HID send */
extern int usbd_send(uint8_t *usbd_send_buf, char *TAG);
extern int usbd_sendRaw(uint8_t buf1, uint8_t buf2, uint8_t buf3, uint8_t buf4, uint8_t buf5, uint8_t buf6, uint8_t buf7, uint8_t buf8);
/* NNK: USB HID API */
extern int usbd_gp_rightrocker_input(uint8_t rx, uint8_t ry);
extern int usbd_gp_leftrocker_input(uint8_t lx, uint8_t ly);
extern int usbd_gp2_input(uint8_t btn);
extern int usbd_gp1_input(uint8_t btn, uint8_t onoff);
extern int usbd_gp0_input(uint8_t btn, uint8_t onoff);
/* NNK: USB HID extra API */
extern int usbd_serialcon_input_handle(uint32_t ValButton, uint8_t ValHAT, uint8_t ValLX, uint8_t ValLY, uint8_t ValRX, uint8_t ValRY);


#endif
