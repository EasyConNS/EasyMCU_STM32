#ifndef _USER_FUNCTION_HANDLE_H_
#define _USER_FUNCTION_HANDLE_H_

/* NNK: User include */
#include "main.h"

/* NNK: Function extern */
/* NNK: 1st API */
extern int delay_ms(int ms);
extern int delay_second(int second);
extern int delay_ms_ps(int ms);
extern int delay_second_ps(int second);
/* NNK: 2nd API */
extern void end_loop(void);

#endif
