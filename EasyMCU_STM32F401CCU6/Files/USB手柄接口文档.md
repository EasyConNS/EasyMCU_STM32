# USBD手柄接口文档

Author: NNK

## 源文件信息

源代码：`项目文件夹/User/Src/usbd_gp_handle.c`

头文件：`项目文件夹/User/Inc/usbd_gp_handle.h`

## 接口/函数信息

### 伊机控专属USBD透传接口

```c
int usbd_serialcon_input_handle(uint32_t ValButton, uint8_t ValHAT, uint8_t ValLX, uint8_t ValLY, uint8_t ValRX, uint8_t ValRY);
```

输入输出

1. 输入：
   * `ValButton`
   * `ValHAT`
   * `ValLX`
   * `ValLY`
   * `ValRX`
   * `ValRY`
2. 输出：
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
uint32_t Button = 0;
uint8_t HAT = 0;
uint8_t LX = 0;
uint8_t LY = 0;
uint8_t RX = 0;
uint8_t RY = 0;		

usbd_serialcon_input_handle(Button, HAT, LX, LY, RX, RY);	//初始化并装载变量，变量直接传参进函数
```

### 按键0区控制

```c
int usbd_gp0_input(uint8_t btn, uint8_t onoff);
```

输入输出

1. 输入：
   * `btn`: 按钮，枚举方式输入
     * y
     * b
     * a
     * x
     * l
     * r
     * zl
     * zr
   * `onoff`: 按钮是否按下，按下输入`1`，释放输入`0`
2. 输出：
   * `USBD_INPUT_OUT_OF_RANGE` - 超出输入范围（枚举：-1）
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp0_input(y, 1);		//按下y按键
```

```c
usbd_gp0_input(r, 0);		//松开r按键
```

### 按键1区控制

```c
int usbd_gp1_input(uint8_t btn, uint8_t onoff);
```

输入输出

1. 输入：
   * `btn`: 按钮，枚举方式输入
     * minus
     * plus
     * l3
     * r3
     * home
     * screenshot
   * `onoff`: 按钮是否按下，按下输入`1`，释放输入`0`
2. 输出：
   * `USBD_INPUT_OUT_OF_RANGE` - 超出输入范围（枚举：-1）
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp1_input(minus, 1);			//按下-按键
```

```c
usbd_gp1_input(screenshot, 0);		//松开截图按键
```

### 按键2区控制

```c
int usbd_gp2_input(uint8_t btn);
```

输入输出

1. 输入：
   * `btn`: 按钮，枚举方式输入
     * up
     
     * upper_right
     
     * right
     
     * lower_right
     
     * down
     
     * lower_left
     
     * left
     
     * upper_left
     
     * center
2. 输出：
   * `USBD_INPUT_OUT_OF_RANGE` - 超出输入范围（枚举：-1）
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp2_input(up, 1);		//按下上按键
```

```c
usbd_gp2_input(lower_left, 0);		//松开左下按键
```

### 左摇杆控制

```c
int usbd_gp_leftrocker_input(uint8_t lx, uint8_t ly);
```

输入输出

1. 输入：
   * `lx`: 0为最左边，128为中间，255为最右边
   * `ly`: 0为最上边，128为中间，255为最下边
   * 可组合使用
2. 输出：
   * `USBD_INPUT_OUT_OF_RANGE` - 超出输入范围（枚举：-1）
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp_leftrocker_input(255, 255);			//左摇杆移至右下方
```

```c
usbd_gp_leftrocker_input(128, 255);			//左摇杆移至正下方
```

### 右摇杆控制

```c
int usbd_gp_rightrocker_input(uint8_t rx, uint8_t ry);
```

输入输出

1. 输入：
   * `rx`: 0为最左边，128为中间，255为最右边
   * `ry`: 0为最上边，128为中间，255为最下边
   * 可组合使用
2. 输出：
   * `USBD_INPUT_OUT_OF_RANGE` - 超出输入范围（枚举：-1）
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp_rightrocker_input(255, 255);			//右摇杆移至右下方
```

```c
usbd_gp_rightrocker_input(128, 255);			//右摇杆移至正下方
```

### 手柄信息初始化

```c
int usbd_gp_init(void);
```

输入输出

1. 输入：
   * 无
2. 输出：
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_gp_init();			//手柄信息初始化
```

### USBD原始数据透传（调试用）

```c
int usbd_sendRaw(uint8_t buf1, uint8_t buf2, uint8_t buf3, uint8_t buf4, uint8_t buf5, uint8_t buf6, uint8_t buf7, uint8_t buf8);
```

输入输出

1. 输入：
   * `buf1`, `buf2`, `buf3`, `buf4`, `buf5`, `buf6`, `buf7`, `buf8`
2. 输出：
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_raw_send(00, 00, 08, 80, 80, 80, 80, 00);		//通过USBD发送8字节原始信息
```

### USBD数据发送

```c
int usbd_send(uint8_t *usbd_send_buf, uint8_t *TAG);
```

输入输出

1. 输入：
   * `usbd_send_buf`: 发送的数组，数组大小需要为8
   * `TAG`: 发送的usbd_send_buf的性质
     * `TAG_SEND_BUF`: 发送全局数组`send_buf[8]`
     * `TAG_SEND_RAW_BUF`: 发送原生数据
     * `TAG_SEND_INIT_BUF`: 发送初始化数组`send_initbuf[8]`
2. 输出：
   * `USBD_SEND_BUSY` - 发送信息忙碌（枚举：0）
   * `USBD_SEND_OK` - 发送信息成功（枚举：1）

示例：

```c
usbd_send(send_buf, TAG_SEND_BUF);		//发送全局数组send_buf[8]
```