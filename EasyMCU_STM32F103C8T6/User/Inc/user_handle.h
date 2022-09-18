#ifndef _USER_HANDLE_H_
#define _USER_HANDLE_H_

/* NNK: enum for DEV_MODE */
enum DEV_CODE
{
    easycon_mode, local_mode 
};

/* NNK: User include */
#include "main.h"

/* NNK: Function extern */
extern int user_entry(void);

/* NNK: User define */
#define DEV_MODE            easycon_mode
#define EASYCON_VERSION     0x45

/* NNK: Board define */

#endif
