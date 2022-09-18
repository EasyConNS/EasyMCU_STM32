#include "uart2_main_handle.h"

const char *TAG_UMAIN   = "[UMAIN]";
const char *TAG_CMD     = "[CMD]";
const char *TAG_RECV    = "-> recv";
const char *TAG_SEND    = "<- send";

/* NNK: 主串口中断函数(HAL_UART_Receive_IT)用变量(1byte) */
uint8_t rxTemp = 0;			                //通过HAL库的串口中断函数获得的1字节数据

/* NNK: 主串口处理函数(umain_handle)用变量 */
char rxCache = 0;                           //通过环形存储区(ringbuffer)得到的一字节缓存
uint8_t rxBuffer[recvBufferSize] = {0};     //接收数组(阻塞)
uint8_t rxTail = 0;							//接收回合，在回调函数中与rxBuffer相关->控制在第(rxTail+1)回合中将rxTemp写入到rxBuffer[rxTail]中
uint8_t rxScript_Flag = 0;                  //伊机控用标志位：标记设备脚本接收状态：0：设备未进入脚本接收状态； 1：设备进入脚本接收状态

/* NNK: 主串口脚本接收函数(umain_script_handle)用变量 */
uint16_t rxScAddr = 0;                      //脚本接收起始地址
uint16_t rxScLen = 0;                       //脚本接收长度
uint16_t rxScAddr_last = 0xFFFF;            //最后一次脚本接收的起始地址，用于做写入flash数据过滤逻辑

/* NNK: 主串口处理函数用变量 */
uint32_t Button = 0;
uint8_t HAT = 0;
uint8_t LX = 0;
uint8_t LY = 0;
uint8_t RX = 0;
uint8_t RY = 0;

/* NNK: 主串口发送函数 */
/* NNK: uint8_t型数组发送函数 */
int umain_send8B(uint8_t *sendbuf, uint8_t len)
{
    int ret = 0;
    printf("%s %s ", TAG_UMAIN, TAG_SEND);
    for (int i = 0; i < len; i++)
    {
        printf(" %02x", sendbuf[i]);
        ret = HAL_UART_Transmit(&huart2, &sendbuf[i], sendBufferSize, 2);
    }
    printx("");
    return ret;
}

/* NNK: 伊机控专用指令发送函数 */
int umain_sendCmd(uint32_t cmd)
{
    uint8_t sendbuf = (uint8_t)cmd;
    int ret = HAL_UART_Transmit(&huart2, &sendbuf, sendBufferSize, 2);
    printx("%s %s %s: %02x", TAG_UMAIN, TAG_SEND, TAG_CMD, sendbuf);
    return ret;
}

/* NNK: 主串口接收函数 */
/* NNK: 主串口中断接收初始化函数 */
void umain_nvic_init(void)
{
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);    //NNK: 设置中断优先级
	HAL_NVIC_EnableIRQ(USART2_IRQn);            //NNK: 使能中断
	HAL_UART_Receive_IT(&huart2, &rxTemp, 1);   //NNK: 开启串口中断接收
	printx("%s nvic init ok", TAG_UMAIN);
}

/* NNK: 主串口中断接收回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart2)
    {
        printx("[%x]", rxTemp);
        user_ringBufferData_in(rxTemp);             //NNK: 一接到串口数据马上将数据放入ringbuffer
        HAL_UART_Receive_IT(&huart2, &rxTemp, 1);   //NNK: 开启串口中断接收
    }
}

/* NNK: 主串口处理函数 */
int umain_handle(void)
{
    if(user_ringBufferData_out(&rxCache) == 1)
    {
        rxBuffer[rxTail] = rxCache;
        printx("[NNK] IN %d %2x %2x %d", rxTail, rxBuffer[0], rxBuffer[rxTail], rxScript_Flag);   //NNK: 测试通讯用调试信息

        if( (rxTail == 1) &&                                            //NNK: 判断1->CMD: 常规CMD判断接收
            (rxBuffer[0] == CMD_READY) && 
            (!rxScript_Flag) &&
            ((rxBuffer[rxTail] == CMD_DEBUG) || 
            (rxBuffer[rxTail] == CMD_HELLO) ||
            (rxBuffer[rxTail] == CMD_SCRIPTSTART) || 
            (rxBuffer[rxTail] == CMD_SCRIPTSTOP) || 
            (rxBuffer[rxTail] == CMD_VERSION)))
        {
            int cmd = rxBuffer[rxTail];                                 //NNK: 由于rxTail要提前清零，所以要提前转储rxBuffer[rxTail]变量
            rxTail = 0;                                                 //NNK: 如果rxTail在cmd处理函数后面清零，会产生1字节的时延偏差。所以要在cmd处理函数前清零
            umain_cmd_handle(cmd);                                      //NNK: cmd处理函数
        }
        else if((rxTail == 2) &&                                        //NNK: 判断2->CMD: 开机握手CMD_HELLO判断接收
                (rxBuffer[0] == CMD_READY) &&
                (rxBuffer[1] == CMD_READY) &&
                (rxBuffer[rxTail] == CMD_HELLO))
        {
            int cmd = rxBuffer[rxTail];                                 //NNK: 由于rxTail要提前清零，所以要提前转储rxBuffer[rxTail]变量
            rxTail = 0;                                                 //NNK: 如果rxTail在cmd处理函数后面清零，会产生1字节的时延偏差。所以要在cmd处理函数前清零
            umain_cmd_handle(cmd);                                      //NNK: cmd处理函数
        }
        else if((rxTail == 5) &&                                        //NNK: 判断3->CMD: 脚本CMD判断接收
                (rxBuffer[0] == CMD_READY) && 
                (!rxScript_Flag) &&
                (rxBuffer[rxTail] == CMD_FLASH) &&
                (!ecScript_Flag))
        {
            int cmd = rxBuffer[rxTail];                                 //NNK: 由于rxTail要提前清零，所以要提前转储rxBuffer[rxTail]变量
            rxTail = 0;                                                 //NNK: 如果rxTail在cmd处理函数后面清零，会产生1字节的时延偏差。所以要在cmd处理函数前清零
            umain_cmd_handle(cmd);                                      //NNK: cmd处理函数
        }
        else if((rxTail == recvPassThroughSize) &&                      //NNK: 判断4->透传: 透传信息判断接收
                (!rxScript_Flag) &&
                (!ecScript_Flag))             
        {
            rxTail = 0;                                                 //NNK: 如果rxTail在cmd处理函数后面清零，会产生1字节的时延偏差。所以要在cmd处理函数前清零
            umain_trans_handle();                                       //NNK: 透传处理函数
        }
        else if((rxTail == (rxScLen - 1)) &&                            //NNK: 判断5->脚本: 脚本信息判断接收
                rxScript_Flag &&
                (!ecScript_Flag))             
        {
            rxTail = 0;                                                 //NNK: 如果rxTail在cmd处理函数后面清零，会产生1字节的时延偏差。所以要在cmd处理函数前清零
            umain_script_handle();                                      //NNK: 脚本处理函数
        }
        else
        {
            rxTail++;
        }
    }
    return 1;
}

/* NNK: 主串口逻辑层处理函数 */
/* NNK: 主串口基本命令(除脚本接收处理命令)处理函数 */
void umain_cmd_handle(int cmd)
{
    printx("%s %s %s: %02x", TAG_UMAIN, TAG_RECV, TAG_CMD, cmd);

    if(!ecScript_Flag)
    {
        switch(cmd)
        {
            case CMD_DEBUG:
                break;

            case CMD_HELLO:
                umain_sendCmd(REPLY_HELLO);
                break;

            case CMD_FLASH:
                rxScAddr = rxBuffer[1] + (rxBuffer[2] << 7);            //NNK: 计算相对位置
                rxScLen = rxBuffer[3] + (rxBuffer[4] << 7);             //NNK: 计算单个接收包长度
                rxScript_Flag = 1;                                      //NNK: 脚本接收标志位，置1
                umain_sendCmd(REPLY_FLASHSTART);
                break;

            case CMD_SCRIPTSTART:
                ecScript_start();
                umain_sendCmd(REPLY_SCRIPTACK);
                break;

            case CMD_VERSION:
                umain_sendCmd(EASYCON_VERSION);
                break;

            default:
                umain_cmd_handle(REPLY_ERROR);
                printx("%s check cmd %d", TAG_UMAIN, cmd);
                break;
        }
    }
    else
    {
        switch(cmd)
        {
            case CMD_SCRIPTSTOP:
                ecScript_stop();
                umain_sendCmd(REPLY_SCRIPTACK);
                break;

            default:
                umain_cmd_handle(REPLY_BUSY);
                break;
        }
    }
}

/* NNK: 主串口指令透传处理函数 */
void umain_trans_handle(void)
{
    printx("%s %s: %02x %02x %02x %02x %02x %02x %02x %02x", TAG_UMAIN, TAG_RECV, rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7], rxBuffer[8]);

    Button = (rxBuffer[0] << 9) | (rxBuffer[1] << 2) | (rxBuffer[2] >> 5);
    HAT = (uint8_t)((rxBuffer[2] << 3) | (rxBuffer[3] >> 4));
    LX = (uint8_t)((rxBuffer[3] << 4) | (rxBuffer[4] >> 3));
    LY = (uint8_t)((rxBuffer[4] << 5) | (rxBuffer[5] >> 2));
    RX = (uint8_t)((rxBuffer[5] << 6) | (rxBuffer[6] >> 1));
    RY = (uint8_t)((rxBuffer[6] << 7) | (rxBuffer[7] & 0x7f));

    //NNK: USBD发送变量
    usbd_serialcon_input_handle(Button, HAT, LX, LY, RX, RY);
    umain_sendCmd(REPLY_ACK);
}

/* NNK: 主串口脚本接收处理函数 */
/* NNK: 经计算和评估，兼容目前的伊机控最大可使用2K字节的flash，共2个flash扇区，合计2048个可用空间 */
/* NNK: 目前这段逻辑的话有一个编译烧录覆写的问题，如果原本脚本的flash的地方有内容，则需要点伊机控的“清除烧录”再点“编译并烧录”才可以完成新的脚本烧写 */
void umain_script_handle(void)
{
    if((rxScAddr == rxScAddr_last) && (rxScAddr != 0))                              //NNK: 脚本重复性检测
    {
        printx("%s %s same Addr %d", TAG_UMAIN, TAG_RECV, rxScAddr);
    }
    else
    {
        if((rxScLen == 2) && (rxBuffer[0] == 0xFF) && (rxBuffer[1] == 0xFF))        //NNK: 如果接收到了清除flash的指令
        {
            flash_ecScript_resetWriteAddr();                                              //NNK: 重置伊机控脚本记录的flash地址位置
            flash_writeErase(ecMemAddr_Start, 2);                                 //NNK: 如果擦除flash太多，会造成擦写时间缓慢，导致发送伊机控命令慢而报错，实测顶多同时擦除两页，多了就报错了
            printx("%s %s flash trimmed", TAG_UMAIN, TAG_RECV);
        }
        else
        {
            if(rxScAddr == 0x0000)                                                  //NNK: 如果收到地址为起始地址
            {
                flash_ecScript_resetWriteAddr();                                          //NNK: 重置伊机控脚本记录的flash地址位置
            }

            rxScAddr_last = rxScAddr;                                               //NNK: 设置脚本重复性检测值
            printf("%s %s [%d]SC[%d]:", TAG_UMAIN, TAG_RECV, rxScAddr, rxScLen);
            for (int i = 0; i < rxScLen; i++)
            {
                printf(" %02x", rxBuffer[i]);
            }
            printf("\r\n");

            flash_ecScript_write8B(rxBuffer, rxScLen, rxScAddr);                       //NNK: 写脚本到flash中
        }
    }

    rxScript_Flag = 0;                                                              //NNK: 脚本接收标志位，置0
    umain_sendCmd(REPLY_FLASHEND);
}
