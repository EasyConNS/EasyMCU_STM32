#ifndef _USER_SCRIPT_HANDLE_H_
#define _USER_SCRIPT_HANDLE_H_

/* NNK: std define */
#include <stdbool.h>

/* NNK: User include */
#include "main.h"
#include "user_library_binary.h"

/* NNK: User define */
#ifdef STM32F103xB
#define MEM_SIZE 900
#elif  STM32F103x6
#define MEM_SIZE 900
#elif  STM32F401xC
#define MEM_SIZE 900
#elif  STM32F411xE
#define MEM_SIZE 900
#endif

/* NNK: Function extern */
/* NNK: easycon script init */
extern void ecScript_init(void);
/* NNK: easycon script API */
extern void ecScript_start(void);
extern void ecScript_stop(void);
/* NNK: easycon script handle */
extern int ecScript_handle(void);

extern uint8_t ecScript_Flag;
extern volatile uint32_t timer_ms;
extern volatile uint32_t wait_ms;

#endif
