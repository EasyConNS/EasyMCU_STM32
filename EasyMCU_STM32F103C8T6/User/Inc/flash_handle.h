#ifndef _FLASH_HANDLE_H_
#define _FLASH_HANDLE_H_

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

/* NNK: User define */
#ifdef STM32F103xB              //NNK: C8T6内存地址，大小64K
#define userMemAddr_Start       (uint32_t)0x08010000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x0801FFFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)1024                              //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)64                                //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x400                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)    //NNK: 伊机控脚本在flash中的开始位置
#elif STM32F103x6               //NNK: C6T6内存地址，大小2K字节(如果C6T6需要移植C8T6的工程，需要移除所有printf和printx，完全擦除flash烧录并检查单片机的flash空间)
#define userMemAddr_Start       (uint32_t)0x08006000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x08007FFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)1024                              //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)64                                //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x400                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)    //NNK: 伊机控脚本在flash中的开始位置
#endif

#define ecScriptData_Len        10                                          //NNK: 伊机控脚本数组长度

/* NNK: Backup flash define */
#define userVal_Offset          4                                           //NNK备用: 用户自定义变量偏差值
#define ecVal_seed_Offset       0                                           //NNK: 伊机控脚本ec_seed位置
//NNK: 下一个变量偏差值增加32

/* NNK: easycon write script event enum */
enum WRITE_SCRIPT_ENUM
{
    WRITE_OK,
    WRITE_FAIL,
    WRITE_ADDR_ERROR
};

/* NNK: Function extern */
/* NNK: Flash write */
extern int flash_writeUnlock(void);
extern int flash_writeErase(uint32_t write_addr, uint32_t pageNum);
extern int flash_write32B(uint32_t write_addr, uint64_t data);
extern int flash_write16B(uint32_t write_addr, uint64_t data);
extern int flash_writeLock(void);
/* NNK: Flash read */
extern int flash_read8B(uint32_t read_addr);
extern int flash_read16B(uint32_t read_addr);
extern int flash_read32B(uint32_t read_addr);
/* NNK: EC Flash API */
extern int flash_ecScript_write8B(uint8_t *data, uint16_t len, uint16_t write_addr);
extern void flash_ecScript_resetWriteAddr(void);
extern int flash_ecScript_read8B(uint16_t read_addr);
/* NNK: Backup Flash API */
extern void flash_userVal_write(void);
extern void flash_userVal_init(void);

extern uint32_t fl_ecScWrite_Addr;
extern uint8_t fl_LockState_Flag;

extern uint16_t ec_seed;

#endif
