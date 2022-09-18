/**
    @brief flash处理注意事项
    @author NNK

    @attention
    如果需要使用此区域的函数，意味着调用接口的操作将不受程序框架的保护，请谨慎操作。
    如果写地址刚好在程序区，基本都会使单片机变砖。
    如果是写自己的地址，确保地址在[userMemAddr_Start, userMemAddr_End]区间内，该区间信息详见flash_handle.h
    写flash操作步骤:1. flash解锁            ->int flash_writeUnlock(void);
                    2. 擦除写地址对应扇区   ->int flash_writeErase(uint32_t prepareAddr);
                    3. 对写地址进行写操作   ->int flash_write32B(uint32_t addr, uint64_t data);
                    4. flash上锁           ->int flash_writeLock(void);
*/

#include "flash.h"

const char *TAG_FLASH   = "[FLASH]";

/* NNK: 伊机控写脚本(flash_ecScript_write8B)用变量 */
uint32_t fl_ecScWrite_Addr = ecMemAddr_Start;                //NNK: 伊机控脚本写flash当前记录的内存位置(绝对位置)，初始化值为easyconAddr_start
uint8_t fl_LockState_Flag = 0;                                   //flash状态标志位：0：flash处于上锁状态；1：flash处于解锁状态

/* NNK: flash底层处理函数 */
/* NNK: flash写解锁函数 */
int flash_writeUnlock(void)
{
		if(fl_LockState_Flag)
			return fl_LockState_Flag;
    fl_LockState_Flag = 1;                                       //NNK: 标志flash已处于解锁状态
    return HAL_FLASH_Unlock();                              //NNK: 解锁flash    
}

#ifdef STM32F401xC
uint8_t flash_getFlashSector(uint32_t erase_addr)
{
    if(erase_addr < ADDR_FLASH_SECTOR_1)
        return FLASH_SECTOR_0;
    else if(erase_addr < ADDR_FLASH_SECTOR_2)
        return FLASH_SECTOR_1;
    else if(erase_addr < ADDR_FLASH_SECTOR_3)
        return FLASH_SECTOR_2;
    else if(erase_addr < ADDR_FLASH_SECTOR_4)
        return FLASH_SECTOR_3;
    else if(erase_addr < ADDR_FLASH_SECTOR_5)
        return FLASH_SECTOR_4;        
    else if(erase_addr < ADDR_FLASH_SECTOR_6)
        return FLASH_SECTOR_5;
    return HAL_ERROR;
}
#elif STM32F411xE
uint8_t flash_getFlashSector(uint32_t erase_addr)
{
    if(erase_addr < ADDR_FLASH_SECTOR_1)
        return FLASH_SECTOR_0;
    else if(erase_addr < ADDR_FLASH_SECTOR_2)
        return FLASH_SECTOR_1;
    else if(erase_addr < ADDR_FLASH_SECTOR_3)
        return FLASH_SECTOR_2;
    else if(erase_addr < ADDR_FLASH_SECTOR_4)
        return FLASH_SECTOR_3;
    else if(erase_addr < ADDR_FLASH_SECTOR_5)
        return FLASH_SECTOR_4;        
    else if(erase_addr < ADDR_FLASH_SECTOR_6)
        return FLASH_SECTOR_5;
    else if(erase_addr < ADDR_FLASH_SECTOR_7)
        return FLASH_SECTOR_6;        
    else if(erase_addr < ADDR_FLASH_SECTOR_8)
        return FLASH_SECTOR_7;    
    return HAL_ERROR;
}
#endif

/* NNK: flash擦除函数 */
int flash_writeErase(uint32_t write_addr, uint32_t pageNum)
{
    FLASH_EraseInitTypeDef f;                               //NNK: 初始化并配置擦除函数所用的指定结构体
#ifdef STM32F103xB
    f.TypeErase = FLASH_TYPEERASE_PAGES;                    //NNK: 擦除类型
    f.PageAddress = write_addr;                             //NNK: 擦除当前地址
    f.NbPages = pageNum;                                    //NNK: 擦除地址的当前页
#elif STM32F103x6
    f.TypeErase = FLASH_TYPEERASE_PAGES;                    //NNK: 擦除类型
    f.PageAddress = write_addr;                             //NNK: 擦除当前地址
    f.NbPages = pageNum;                                    //NNK: 擦除地址的当前页
#elif STM32F401xC
    f.TypeErase = FLASH_TYPEERASE_SECTORS;
    f.Sector = flash_getFlashSector(write_addr);
    f.NbSectors = pageNum;
    f.VoltageRange = FLASH_VOLTAGE_RANGE_3;
#endif    
    uint32_t PageError = 0;                                 //NNK: 设置PageError变量，使调用了擦除函数后可以判断是否有扇区错误
    HAL_FLASHEx_Erase(&f, &PageError);                      //NNK: 使用配置f擦除flash，擦除对象位擦除地址的整个当前页
    return PageError;
}


/* NNK: flash写uint16_t数据函数 */
int flash_write16B(uint32_t write_addr, uint64_t data)
{
    return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, write_addr, data); //NNK: 写flash
}

/* NNK: flash写上锁函数 */
int flash_writeLock(void)
{
		if(fl_LockState_Flag==0)
			return fl_LockState_Flag;
    fl_LockState_Flag = 0;                                  //NNK: 标志flash已处于上锁的状态
    return HAL_FLASH_Lock();                                //NNK: flash上锁
}

/* NNK: 伊机控flash接口函数 */
/* NNK: 写伊机控脚本函数-通过长度 */
int flash_ecScript_write8B(uint8_t *data, uint16_t len, uint16_t write_addr)
{
	  uint16_t fl_easyconData_Len = 0;
    uint8_t ret = 0;
	
		if(write_addr % 2 == 1)
			return WRITE_FAIL;
	
		fl_ecScWrite_Addr = ecMemAddr_Start + write_addr;
	
		fl_easyconData_Len = len / 2;
    for (uint16_t i = 0; i < fl_easyconData_Len; i++)
    {
        ret = flash_write16B(fl_ecScWrite_Addr, data[2*i]|data[2*i+1]<<8);
        if(ret != HAL_OK)
        {
            return WRITE_FAIL;
        }
        fl_ecScWrite_Addr += 2;
    }
		if(len % 2 == 1)
		{
			ret = flash_write16B(fl_ecScWrite_Addr, data[len-1]|0xFF<<8);
			if(ret != HAL_OK)
			{
					return WRITE_FAIL;
			}
			fl_ecScWrite_Addr += 2;     
		}

    return WRITE_OK;
}

/* NNK: 读uint8_t函数-原始数据 */
uint8_t flash_ecScript_read8B(uint16_t read_addr)
{
    return *((volatile uint8_t *)(read_addr + ecMemAddr_Start));
}

/* NNK: 读uint8_t函数-原始数据 */
uint16_t flash_ecScript_read16B(uint16_t read_addr)
{
    return *((volatile uint16_t *)(read_addr + ecMemAddr_Start));
}

/* NNK: 读uint8_t函数-原始数据 */
void flash_ecScript_read(uint8_t *data, uint16_t len, uint16_t read_addr)
{
    for(uint16_t i=0;i<len;i++)
    {
        data[i] = *((volatile uint8_t *)(read_addr + ecMemAddr_Start+i));
    }
}
