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

#include "flash_handle.h"

const char *TAG_FLASH   = "[FLASH]";

/* NNK: 伊机控写脚本(flash_ecScript_write8B)用变量 */
uint32_t fl_ecScWrite_Addr = ecMemAddr_Start;                //NNK: 伊机控脚本写flash当前记录的内存位置(绝对位置)，初始化值为easyconAddr_start
uint8_t fl_LockState_Flag = 0;                                   //flash状态标志位：0：flash处于上锁状态；1：flash处于解锁状态

/* NNK: 用户自定义读写用变量 */
uint16_t ec_seed = 0;

/* NNK: flash底层处理函数 */
/* NNK: flash写解锁函数 */
int flash_writeUnlock(void)
{
    fl_LockState_Flag = 1;                                       //NNK: 标志flash已处于解锁状态
    return HAL_FLASH_Unlock();                              //NNK: 解锁flash    
}

/* NNK: flash擦除函数 */
int flash_writeErase(uint32_t write_addr, uint32_t pageNum)
{
    FLASH_EraseInitTypeDef f;                               //NNK: 初始化并配置擦除函数所用的指定结构体
    f.TypeErase = FLASH_TYPEERASE_PAGES;                    //NNK: 擦除类型
    f.PageAddress = write_addr;                                   //NNK: 擦除当前地址
    f.NbPages = pageNum;                                    //NNK: 擦除地址的当前页
    uint32_t PageError = 0;                                 //NNK: 设置PageError变量，使调用了擦除函数后可以判断是否有扇区错误
    HAL_FLASHEx_Erase(&f, &PageError);                      //NNK: 使用配置f擦除flash，擦除对象位擦除地址的整个当前页
    return PageError;
}

/* NNK: flash写uint32_t数据函数 */
int flash_write32B(uint32_t write_addr, uint64_t data)
{
    return HAL_FLASH_Program(TYPEPROGRAM_WORD, write_addr, data); //NNK: 写flash
}

/* NNK: flash写uint16_t数据函数 */
int flash_write16B(uint32_t write_addr, uint64_t data)
{
    return HAL_FLASH_Program(TYPEPROGRAM_HALFWORD, write_addr, data); //NNK: 写flash
}

/* NNK: flash写上锁函数 */
int flash_writeLock(void)
{
    fl_LockState_Flag = 0;                                       //NNK: 标志flash已处于上锁的状态
    return HAL_FLASH_Lock();                                //NNK: flash上锁
}

/* NNK: 读uint8_t函数-原始数据 */
int flash_read8B(uint32_t read_addr)
{
    return *((volatile char *)(read_addr));
}

/* NNK: 读uint16_t函数-原始数据 */
int flash_read16B(uint32_t read_addr)
{
    return *((volatile short *)(read_addr));
}

/* NNK: 读uint32_t函数-原始数据 */
int flash_read32B(uint32_t read_addr)
{
    return *((volatile int *)(read_addr));
}

/* NNK: 伊机控flash接口函数 */
/* NNK: 写伊机控脚本函数-通过长度 */
int flash_ecScript_write8B(uint8_t *data, uint16_t len, uint16_t write_addr)
{
    if(write_addr != (fl_ecScWrite_Addr - (ecMemAddr_Start)))
    {
        return WRITE_ADDR_ERROR;
    }

    uint8_t fl_easyconData_Len = len / 2;
    uint16_t fl_easyconData[ecScriptData_Len] = {0};                         //NNK: 写入flash数组(处理后)
    uint8_t ret = 0;

    for (int n = 0; n < len; n++)
    {
        fl_easyconData[n / 2] += (data[n] << ((n % 2) * 8));                //NNK: 组合待写入数据，待验证
    }

    for (int i = 0; i < fl_easyconData_Len; i++)
    {
        ret = flash_write16B(fl_ecScWrite_Addr, fl_easyconData[i]);            //NNK: 写flash
        if(ret != HAL_OK)
        {
            return WRITE_FAIL;
        }
        fl_ecScWrite_Addr += 2;                                                //NNK: 由于组合数据成了uint16_t型，故地址偏移自增量为2。全字(uint32_t)偏移量为4
    }

    for (int i = 0; i < fl_easyconData_Len; i++)
    {
        printx("%s WRADDR[%08x] = %08x", TAG_FLASH, fl_ecScWrite_Addr - len + (2 * i), flash_read16B(fl_ecScWrite_Addr - len + (2 * i)));
    } 

    return WRITE_OK;
}

/* NNK: 重置目前easycon的flash写位置函数 */
void flash_ecScript_resetWriteAddr(void)
{
    fl_ecScWrite_Addr = ecMemAddr_Start;
    printx("%s WRADDR reset ok", TAG_FLASH);
}

/* NNK: 读uint8_t函数-原始数据 */
int flash_ecScript_read8B(uint16_t read_addr)
{
    return *((volatile char *)(read_addr + ecMemAddr_Start));
}

/* NNK: 备用flash接口函数 */
/* NNK: 用户变量写入函数 */
void flash_userVal_write(void)
{
    flash_writeErase(userMemAddr_Start, 1);

    //NNK: 用户数据写入
    flash_write32B(userMemAddr_Start + ecVal_seed_Offset, (uint32_t)ec_seed);

    printx("%s Val write ok", TAG_FLASH);
}

/* NNK: 用户变量初始化函数 */
void flash_userVal_init(void)
{
    ec_seed = (uint16_t)(flash_read32B(userMemAddr_Start + ecVal_seed_Offset)) + 1;
    printx("%s ec_seed = %08x", TAG_FLASH, ec_seed);
}
