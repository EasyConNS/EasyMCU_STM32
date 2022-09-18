# NANcon硬件说明

## 引脚配置

| 功能       | 功能对应                 | 引脚     |
| ---------- | ------------------------ | -------- |
| USB        | USB_DP                   | PA12     |
| USB        | USB_DM                   | PA11     |
| UART1      | DEBUG_TX                 | PA9      |
| ~~UART1~~  | ~~DEBUG_RX~~             | ~~PA10~~ |
| UART2      | MAIN_TX                  | PA2      |
| UART2      | MAIN_RX                  | PA3      |
| 板灯       | GPIO_Pin                 | PB12     |
| TIM3       | 伊机控脚本用软件定时器   | NULL     |
| ringbuffer | 主串口用环形存储区（堆） | NULL     |

# NANcon软件说明

## 工具

### 条件（依赖）

* Keil5 MDK（必须具备）
* 适用于Keil5的STM32F1芯片包（必须具备）
* STLINK V2驱动（可选）
* VSCode（可选）

### Keil5 MDK设置

如果使用Keil5在本工程里面看到了乱码，请将其设置成UTF-8模式

方法传送门：[如何解决keil mdk中文乱码或设置编码问题-百度经验 (baidu.com)](https://jingyan.baidu.com/article/5225f26bb16709e6fa0908ea.html)

本工程的警告屏蔽：

* `警告177`：该警告主要是创建printx的时候，屏蔽`va_list`传值警告，对程序使用一般没有影响
* `警告767`：该警告出现在`项目文件夹/User/user_script_handle.c`中，属于硬移植`SerialCon`代码导致，大致意思是变量隐性强转，数据内容有可能会被截断

### VSCode编辑器设置

如果需要使用VSCode编辑本工程，请在本工程首页``右键空白处``-`通过Code打开`。同时，可能需要编辑`项目文件夹/.vscode/c_cpp_properties.json`文件以告诉VSCode头文件位置和全局宏定义，以下为说明：
* 此文件为Json文件，对格式要求比较严格，格式按照文件内的格式照猫画虎即可。**另外，需要注意编辑路径的时候斜杠`/`与反斜杠`\`的区别，很多不识别或者错误都是因为没有注意这点翻车的**
* `includePath`：头文件路径需要为Keil5的安装目录内的`ARM/CMSIS/**`和`ARM/ARMCC/**`。前者提供单片机内核头文件，后者提供标准输入输出文件（如`stdio.h`等）
* `defines`：全局宏定义需要与Keil5中的全局宏定义一致（如`USE_HAL_DRIVER`和`STM32F103xB`），在此基础上加上`__CC_ARM`以让VSCode识别uint32_t数据类型
  * `__CC_ARM`出处：[vscode开发stm32项目时提示找不到uint32_t类型的解决办法_cp_srd的博客-CSDN博客](https://blog.csdn.net/cp_srd/article/details/110673301)


## NANcon运行模式

本设备有两个模式：

* 伊机控模式：在此模式下，可以与伊机控上位机进行通讯
* 本地模式：在此模式下，可以自定义脚本，并固化在单片机固件内，开机即运行脚本，需要使用本项目提供的接口

如何改变设备运行模式：改变`DEV_MODE`的define值，可更改为

* `easycon_mode`：伊机控模式
* `local_mode`：本地模式

`DEV_MODE`的位置：`项目文件夹/User/Inc/user_handle.h`

## 本地模式

为了方便脚本的编程和单片机运行的稳定性，将在项目文件里面提供一个函数`user_entry`，在此函数里，所有的接口都可以使用。当`DEV_MODE`的值为`local_mode`时，此区域将被激活，开启单片机将默认循环执行`user_entry`。

脚本编写入口`user_entry`位置：`项目文件夹/User/Src/user_handle.c`

脚本编写位置：

```c
int user_entry(void)
{
    /* NNK: 脚本开始 */

    /* NNK: 脚本结束 */
    return 1;
}
```

## 接口说明

### 各外设接口说明

FLASH接口说明：[FLASH接口说明](./Files/FLASH接口文档.md)

USB手柄接口说明：[USB手柄接口说明](./Files/USB手柄接口文档.md)

板灯接口说明：[板灯接口说明](./Files/板灯接口文档.md)

主串口接口说明：[主串口接口说明](./Files/主串口接口文档.md)

用户自定义函数接口文档说明：[用户自定义函数接口文档说明](./Files/用户自定义函数接口文档.md)

### 标准输入输出库函数说明

* fputc/printf可用，输出将用于DEBUG调试串口输出（UART1）
* fgetc/scanf不可用

## 代码可移植性

基本有了这份代码，比STM32F103C6T6要好的STM32单片机都适用

## 项目文件及文件夹结构

详见[tree](./tree.txt)
