#include "uart2.h"
#include "EasyCon_API.h"

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
    for (int i = 0; i < len; i++)
    {
        ret = HAL_UART_Transmit(&huart1, &sendbuf[i], sendBufferSize, 2);
    }
    return ret;
}

/* NNK: 伊机控专用指令发送函数 */
int umain_sendCmd(uint32_t cmd)
{
    uint8_t sendbuf = (uint8_t)cmd;
    int ret = HAL_UART_Transmit(&huart1, &sendbuf, sendBufferSize, 2);
    return ret;
}

/* NNK: 主串口接收函数 */
/* NNK: 主串口中断接收初始化函数 */
void umain_nvic_init(void)
{
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 2);    //NNK: 设置中断优先级
	HAL_NVIC_EnableIRQ(USART1_IRQn);            //NNK: 使能中断
	HAL_UART_Receive_IT(&huart1, &rxTemp, 1);   //NNK: 开启串口中断接收
	
//	HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);    //NNK: 设置中断优先级
//	HAL_NVIC_EnableIRQ(USART2_IRQn);            //NNK: 使能中断
//	HAL_UART_Receive_IT(&huart2, &rxTemp, 1);   //NNK: 开启串口中断接收
}

/* NNK: 主串口中断接收回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
      //user_ringBufferData_in(rxTemp);             //NNK: 一接到串口数据马上将数据放入ringbuffer
      EasyCon_serial_task(rxTemp);  
			HAL_UART_Receive_IT(&huart1, &rxTemp, 1);   //NNK: 开启串口中断接收
    }
}
