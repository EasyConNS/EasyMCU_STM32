#ifndef _FLASH_HANDLE_H_
#define _FLASH_HANDLE_H_

/* NNK: User include */
#include "main.h"

/* NNK: HAL include */
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"

/* NNK: User define */
#ifdef STM32F103xB              //NNK: C8T6内存地址，大小64K字节
#define userMemAddr_Start       (uint32_t)0x08010000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x0801FFFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)1024                              //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)64                                //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x400                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)      //NNK: 伊机控脚本在flash中的开始位置
#define ecMemPageNum            (uint32_t)2                                 //NNK: 伊机控脚本占用的内存页面数量
#elif STM32F103x6               //NNK: C6T6内存地址，大小2K字节(如果C6T6需要移植C8T6的工程，需要移除所有printf和printx，完全擦除flash烧录并检查单片机的flash空间)
#define userMemAddr_Start       (uint32_t)0x08006000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x08007FFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)1024                              //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)2                                 //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x400                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)      //NNK: 伊机控脚本在flash中的开始位置
#define ecMemPageNum            (uint32_t)2                                 //NNK: 伊机控脚本占用的内存页面数量
#elif STM32F401xC               //NNK: CCU6内存地址，大小128K字节
#define userMemAddr_Start       (uint32_t)0x08020000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x0803FFFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)(128 * 1024)                      //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)1                                 //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x000                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)      //NNK: 伊机控脚本在flash中的开始位置
#define ecMemPageNum            (uint32_t)1                                 //NNK: 伊机控脚本占用的内存页面数量
#elif STM32F411xE
#define userMemAddr_Start       (uint32_t)0x08020000                        //NNK: 起始地址
#define userMemAddr_End         (uint32_t)0x0807FFFF                        //NNK: 结束地址
#define userMemPage_Size        (uint32_t)(256 * 1024)                      //NNK: 每页字节数/每页字节大小
#define userMemPageNum          (uint32_t)1                                 //NNK: 可用flash总页数
#define ecMemAddr_Offset        (uint32_t)0x000                             //NNK: 存储伊机控的相对地址偏移位置
#define ecMemAddr_Start         (userMemAddr_Start + ecMemAddr_Offset)      //NNK: 伊机控脚本在flash中的开始位置
#define ecMemPageNum            (uint32_t)1                                 //NNK: 伊机控脚本占用的内存页面数量#endif
#endif

/* NNK: STM32F4 series flash define */
#ifdef STM32F401xC
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000)                      //NNK: 
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000)
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000)
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000)
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000)
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000)
#elif STM32F411xE
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000)                      //NNK: 
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000)
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000)
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000)
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000)
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000)
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000)
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000)
#endif

/* NNK: easycon write script event enum */
enum WRITE_SCRIPT_ENUM
{
    WRITE_OK,
    WRITE_FAIL,
    WRITE_ADDR_ERROR
};

/* NNK: EC Flash API */
extern int flash_writeUnlock(void);
extern int flash_writeLock(void);

extern int flash_writeErase(uint32_t write_addr, uint32_t pageNum);

extern int flash_ecScript_write8B(uint8_t *data, uint16_t len, uint16_t write_addr);
extern int flash_write16B(uint32_t write_addr, uint64_t data);


extern void flash_ecScript_read(uint8_t *data, uint16_t len, uint16_t read_addr);
extern uint8_t flash_ecScript_read8B(uint16_t read_addr);
extern uint16_t flash_ecScript_read16B(uint16_t read_addr);

extern uint32_t fl_ecScWrite_Addr;
extern uint8_t fl_LockState_Flag;

#endif
