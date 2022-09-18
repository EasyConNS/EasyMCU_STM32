# flash接口文档

Author: NNK

## 源文件信息

源代码：`项目文件夹/User/Src/flash_handle.c`

头文件：`项目文件夹/User/Inc/flash_handle.h`

## 接口/函数信息

### flash解锁接口

```c
int flash_writeUnlock(void);
```

输入输出

1. 输入：
   * 无

2. 输出：
   * `HAL_ERROR` - flash解锁失败（枚举：1）
   * `HAL_OK` - flash解锁成功（枚举：0）

示例：

```c
flash_writeUnlock();
```

### flash擦除接口

```c
int flash_writeErase(uint32_t write_addr, uint32_t pageNum);
```

输入输出

1. 输入：
   * `write_addr` - 写入地址
   * `pageNum` - 擦除以写入地址该页开始计算的页数

2. 输出：
   * `PageError` - 擦除失败的地址，若没有擦除失败则为0

示例：

```c
flash_writeErase(0x08010000, 1);	//若该单片机为STM32F103C8T6，则擦除该单片机允许擦除的内存地址开始的第一页内存的所有空间（1页 = 1024字节）
```

### flash写入32字节接口

```c
int flash_write32B(uint32_t write_addr, uint64_t data);
```

输入输出

1. 输入：
   * `write_addr` - 写入地址
   * `data` - 数据

2. 输出：
   * `HAL_ERROR` - flash写入失败（枚举：1）
   * `HAL_OK` - flash写入成功（枚举：0）

示例：

```c
uint32_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
flash_write32B(0x08010000, data[2]);	//一次只能写一个uint32_t数据
```

### flash写入16字节接口

```c
int flash_write16B(uint32_t write_addr, uint64_t data);
```

输入输出

1. 输入：
   * `write_addr` - 写入地址
   * `data` - 数据

2. 输出：
   * `HAL_ERROR` - flash写入失败（枚举：1）
   * `HAL_OK` - flash写入成功（枚举：0）

示例：

```c
uint16_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
flash_write16B(0x08010000, data[2]);	//一次只能写一个uint16_t数据
```

### flash上锁接口

```c
int flash_writeLock(void);
```

输入输出

1. 输入：
   * 无

2. 输出：
   * `HAL_OK` - flash锁上锁成功（枚举：0）

示例：

```c
flash_writeLock();
```

### flash读取8字节数据接口

```c
int flash_read8B(uint32_t read_addr);
```

输入输出

1. 输入：
   * `read_addr` - 读取地址

2. 输出：
   * 读取到的数据，格式为char型(int8_t)

示例：

```c
uint8_t val = 0;
val = (uint8_t)flash_read8B(0x08010000);
```

### flash读取16字节数据接口

```c
int flash_read16B(uint32_t read_addr);
```

输入输出

1. 输入：
   * `read_addr` - 读取地址

2. 输出：
   * 读取到的数据，格式为short型(int16_t)

示例：

```c
uint16_t val = 0;
val = (uint16_t)flash_read16B(0x08010000);
```

### flash读取32字节数据接口

```c
int flash_read32B(uint32_t read_addr);
```

输入输出

1. 输入：
   * `read_addr` - 读取地址

2. 输出：
   * 读取到的数据，格式为int型(int32_t)

示例：

```c
uint32_t val = 0;
val = (uint32_t)flash_read32B(0x08010000);
```

## 伊机控相关接口/函数信息

### flash写入伊机控脚本(8字节)数据接口

```c
int flash_ecScript_write8B(uint8_t *data, uint16_t len, uint16_t write_addr)
```

输入输出

1. 输入：
   * `*data` - 脚本一次性接收的`len`长度的uint8_t型脚本数组
   * `len` - 脚本数组长度
   * `write_addr` - 脚本数组写入的地址

2. 输出：
   * `WRITE_ADDR_ERROR` - flash写入地址错误（枚举：2）
   * `WRITE_FAIL` - flash写入错误（枚举：1）
   * `WRITE_OK` - flash写入成功（枚举：0）

### flash重置伊机控脚本写入地址接口

```c
void flash_ecScript_resetWriteAddr(void)
```

输入输出

1. 输入：
   * 无

2. 输出：
   * 无

### flash读取伊机控脚本(8字节)数据接口

```c
int flash_ecScript_read8B(uint16_t read_addr)
```

输入输出

1. 输入：
   * `read_addr` - 读取伊机控脚本的地址

2. 输出：
   * 该地址的char类型(int8_t)的数据

## 用户自定义数据相关接口/函数信息

### flash写入用户自定义数据(32字节)数据接口

```c
void flash_userVal_write(void)

```

**需要用户自行编辑该函数**

输入输出

1. 输入：
   * 空
2. 输出：
   * 空

### flash初始化用户自定义数据(32字节)数据接口

```c
void flash_userVal_init(void)

```

**需要用户自行编辑该函数**

输入输出

1. 输入：
   * 空
2. 输出：
   * 空