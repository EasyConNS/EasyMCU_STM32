#ifndef _USER_RINGBUFFER_HANDLE_H_
#define _USER_RINGBUFFER_HANDLE_H_

/* NNK: std define */
#include <stdlib.h>

/* NNK: User include */
#include "main.h"

/* NNK: Function extern */
/* NNK: ringbuffer init */
extern int user_ringbuffer_init(int size);
/* NNK: ringbuffer in */
extern int user_ringBufferData_in(char inData);
/* NNK: ringbuffer out */
extern int user_ringBufferData_out(char *outData);

#endif
