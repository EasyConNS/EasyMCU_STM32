# 硬件说明

## 引脚配置

| 功能  | 功能对应               | 引脚 |
| ----- | ---------------------- | ---- |
| USB   | USB_DP                 | PA12 |
| USB   | USB_DM                 | PA11 |
| UART1 | MAIN_TX                | PA9  |
| UART1 | MAIN_RX                | PA10 |
| 板灯  | GPIO_Pin               | PC13 |
| TIM3  | 伊机控脚本用软件定时器 | NULL |



# 开发环境

## 依赖

* Keil5 MDK
* 适用于Keil5的STM32F1/F4芯片包



## Keil5设置

如果使用Keil5在本工程里面看到了乱码，请将其设置成UTF-8模式

方法传送门：[如何解决keil mdk中文乱码或设置编码问题-百度经验 (baidu.com)](https://jingyan.baidu.com/article/5225f26bb16709e6fa0908ea.html)
