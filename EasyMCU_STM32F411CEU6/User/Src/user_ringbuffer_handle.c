#include "user_ringbuffer_handle.h"

const char *TAG_RINGBUFFER = "[RINGBUFFER]";

static char *r_pBuffer;
static int r_buffsize;
static int r_writePos;
static int r_readPos;
static int r_dataLen;

int user_ringbuffer_init(int size)
{
    r_buffsize = size;
    r_pBuffer = (char *)malloc(size);
    if(r_pBuffer == NULL)                               //NNK: malloc申请地址检测
    {
        printx("%s rb(%db) init !ok", TAG_RINGBUFFER, size);
        return 0;
    }
    printx("%s rb(%db) init ok", TAG_RINGBUFFER, size);
    return 1;
}

int user_ringBufferData_in(char inData)
{
    if((r_writePos + 1) % r_buffsize == r_readPos)      //NNK: 满栈检测
    {
        return 0;
    }
    r_pBuffer[r_writePos] = inData;
    r_writePos = (r_writePos + 1) % r_buffsize;         //NNK: 获取数据写位置，%r_buffsize是为了防止数据写位置超出环形缓冲区位置
    r_dataLen++;
    return 1;
}

int user_ringBufferData_out(char *outData)
{
    if(r_readPos == r_writePos)                         //NNK: 空栈检测
    {
        return 0;
    }
    *outData = r_pBuffer[r_readPos];
    r_readPos = (r_readPos + 1) % r_buffsize;           //NNK: 获取数据读位置，%r_buffsize是为了防止数据读位置超出环形缓冲区位置
    r_dataLen--;
    return 1;
}
