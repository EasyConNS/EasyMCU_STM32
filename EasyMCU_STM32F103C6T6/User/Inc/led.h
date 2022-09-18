#ifndef _LEDB_HANDLE_H_
#define _LEDB_HANDLE_H_

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

/* NNK: Function extern */
extern void ledb_on(void);
extern void ledb_off(void);
extern void ledb_test(void);

#endif
