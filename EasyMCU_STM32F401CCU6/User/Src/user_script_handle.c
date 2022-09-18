#include "user_script_handle.h"

// constants
#define ECHO_TIMES 3
#define ECHO_INTERVAL 2
#define LED_DURATION 50
#define SERIAL_BUFFER_SIZE 20
#define DIRECTION_OFFSET 20
#define VARSPACE_OFFSET 90
#define KEYCODE_OFFSET 90
#define KEYCODE_MAX 33
#define REGISTER_OFFSET 130
#define STACK_OFFSET 150
#define CALLSTACK_OFFSET 190
#define FORSTACK_OFFSET 250
#define INS_OFFSET 370
#define SEED_OFFSET MEM_SIZE + 0

// indexed variables and inline functions
#define Max(a, b) ((a > b) ? (a) : (b))
#define Min(a, b) ((a < b) ? (a) : (b))
#define SERIAL_BUFFER(i) mem[(i)]
#define KEY(keycode) mem[KEYCODE_OFFSET+(keycode)]
#define REG(i) *(int16_t*)(mem+REGISTER_OFFSET+((i)<<1))
#define STACK(i) *(int16_t*)(mem+STACK_OFFSET+((i)<<1))
#define CALLSTACK(i) *(uint16_t*)(mem+CALLSTACK_OFFSET+((i)<<1))
#define DX(i) mem[DIRECTION_OFFSET+((i)<<1)]
#define DY(i) mem[DIRECTION_OFFSET+((i)<<1)+1]
#define FOR_I(i) *(int32_t*)(mem+FORSTACK_OFFSET+(i)*12)
#define FOR_C(i) *(int32_t*)(mem+FORSTACK_OFFSET+(i)*12+4)
#define FOR_ADDR(i) *(uint16_t*)(mem+FORSTACK_OFFSET+(i)*12+8)
#define FOR_NEXT(i) *(uint16_t*)(mem+FORSTACK_OFFSET+(i)*12+10)
#define SETWAIT(time) wait_ms=(time)
#define RESETAFTER(keycode,n) KEY(keycode)=n
#define JUMP(addr) script_addr = (uint8_t*)(addr)
#define JUMPNEAR(addr) script_addr = (uint8_t*)((uint16_t)script_addr + (addr))
#define E(val) _e_set = 1, _e_val = (val)
#define E_SET ((_e_set_t = _e_set),(_e_set = 0),_e_set_t)

// single-byte variables
#define _ins3 mem[INS_OFFSET]
#define _ins2 mem[INS_OFFSET+1]
#define _ins1 mem[INS_OFFSET+2]
#define _ins0 mem[INS_OFFSET+3]
#define _ins *(uint16_t*)(mem+INS_OFFSET+2)
#define _insEx *(uint32_t*)(mem+INS_OFFSET)
#define _code mem[INS_OFFSET+4]
#define _keycode mem[INS_OFFSET+5]
#define _lr mem[INS_OFFSET+6]
#define _direction mem[INS_OFFSET+7]
#define _addr *(uint16_t*)(mem+INS_OFFSET+8)
#define _stackindex mem[INS_OFFSET+10]
#define _callstackindex mem[INS_OFFSET+11]
#define _forstackindex mem[INS_OFFSET+12]
#define _report_echo mem[INS_OFFSET+13]
#define _e_set mem[INS_OFFSET+14]
#define _e_set_t mem[INS_OFFSET+15]
#define _e_val *(uint16_t*)(mem+INS_OFFSET+16)    // external argument for next instruction (used for dynamic for-loop, wait etc.)
#define _script_running mem[INS_OFFSET+18]
#define _ri0 mem[INS_OFFSET+19]
#define _ri1 mem[INS_OFFSET+20]
#define _v mem[INS_OFFSET+21]
#define _flag mem[INS_OFFSET+22]
#define _seed *(uint16_t*)(mem+INS_OFFSET+23)

/* NNK: User define */
#define STICK_MIN      0
#define STICK_CENTER 128
#define STICK_MAX    255

const char *TAG_SCRIPT = "[SCRIPT]";

void BinaryOp(uint8_t op, uint8_t reg, int16_t value);

/* NNK: 伊机控计时器 */
volatile uint32_t timer_ms = 0;                      // script timer(srand)
volatile uint32_t wait_ms = 0;                       // waiting counter

/* NNK: 伊机控脚本处理函数(ecScript_handle)用变量 */
uint8_t ecScript_Flag = 0;              //NNK: 伊机控脚本运行标志位：0：伊机控脚本未运行；1：初次运行伊机控脚本（即将运行一次初始化函数）；2：伊机控脚本正在运行

/* NNK: 伊机控脚本处理函数(ecScript_handle)用手柄控制变量 */
uint32_t ecScript_Button    = 0x00;
uint8_t ecScript_HAT        = 0x08;
uint8_t ecScript_LX         = 0x80;
uint8_t ecScript_LY         = 0x80;
uint8_t ecScript_RX         = 0x80;
uint8_t ecScript_RY         = 0x80;

// global variables
uint8_t mem[MEM_SIZE] = {0};                            // preallocated memory for all purposes, as well as static instruction carrier
uint8_t* flash_addr = 0;                                // start location for EEPROM flashing
uint16_t flash_index = 0;                               // current buffer index
uint16_t flash_count = 0;                               // number of bytes expected for this time
uint8_t* script_addr = 0;                               // address of next instruction
uint8_t* script_eof = 0;                                 // address of EOF
uint16_t tail_wait = 0;                                   // insert an extra wait before next instruction (used by compressed instruction)
uint32_t timer_elapsed = 0;                          // previous execution time

/* NNK: 伊机控脚本处理函数 */
int ecScript_handle(void)
{

    //NNK: 计时器检查
    if(wait_ms > 0)
    {
        return 1;
    }

    for (uint8_t i = 0; i <= KEYCODE_MAX; i++)
    {
        if(KEY(i) != 0)
        {
            KEY(i)--;
            if(KEY(i) == 0)
            {
                if(i == 32)
                {
                    ecScript_LX = STICK_CENTER;
                    ecScript_LY = STICK_CENTER;
                    _report_echo = ECHO_TIMES;
                }
                else if(i == 33)
                {
                    ecScript_RX = STICK_CENTER;
                    ecScript_RY = STICK_CENTER;
                    _report_echo = ECHO_TIMES;
                }
                else if((i & 0x10) == 0)
                {
                    ecScript_Button &= ~(1 << i);
                    _report_echo = ECHO_TIMES;
                }
                else
                {
                    ecScript_HAT = HAT_CENTER;
                    _report_echo = ECHO_TIMES;
                }
            }
        }
    }

    if(tail_wait != 0)
    {
        //easycon: wait after compressed instruction
        SETWAIT(tail_wait);
        tail_wait = 0;
        usbd_serialcon_input_handle(ecScript_Button, ecScript_HAT, ecScript_LX, ecScript_LY, ecScript_RX, ecScript_RY);
        return 1;
    }

    _addr = (uint16_t)script_addr;
    _ins0 = flash_ecScript_read8B((uint16_t)script_addr++);
    _ins1 = flash_ecScript_read8B((uint16_t)script_addr++);
    
    if((_ins0 == 0xFF) && (_ins1 == 0xFF))
    {
        ecScript_stop();
        return 1;
    }

    int32_t n;
    int16_t reg;
    if(_ins0 & B10000000)
    {
        //easycon: key/stick actions
        if((_ins0 & B01000000) == 0)
        {
            //easycon: Instruction : Key
            _keycode = (_ins0 >> 1) & B00011111;

            //easycon: modify report
            if((_keycode & 0x10) == 0)
            {
                //easycon: Button
                ecScript_Button |= 1 << _keycode;
                _report_echo = ECHO_TIMES;
            }
            else
            {
                //easycon: HAT
                ecScript_HAT = _keycode & 0xF;
                _report_echo = ECHO_TIMES;
            }

            //easycon: post effect
            if(E_SET)
            {
                //easycon: pre-loaded duration
                SETWAIT(REG(_e_val));
                RESETAFTER(_keycode, 1);
            }
            else if((_ins0 & B00000001) == 0)
            {
                //easycon: standard
                n = _ins1;
                //easycon: unscale
                n *= 10;
                SETWAIT(n);
                RESETAFTER(_keycode, 1);
            }
            else if((_ins1 & B10000000) == 0)
            {
                //easycon: compressed
                tail_wait = _ins1 & B01111111;
                //easycon: unscale
                tail_wait *= 50;
                SETWAIT(50);
                RESETAFTER(_keycode, 1);
            }
            else
            {
                //easycon: hold
                n = _ins1 & B01111111;
                RESETAFTER(_keycode, n);
            }
        }
        else
        {
            //easycon: Instruction : Stick
            _lr = (_ins0 >> 5) & 1;
            _keycode = 32 | _lr;
            _direction = _ins0 & B00011111;
            //easycon: modify report
            if(_lr)
            {
                //easycon: RS
                ecScript_RX = DX(_direction);
                ecScript_RY = DY(_direction);
                _report_echo = ECHO_TIMES;
            }
            else
            {
                //easycon: LS
                ecScript_LX = DX(_direction);
                ecScript_LY = DY(_direction);
                _report_echo = ECHO_TIMES;
            }

            //easycon: post effect
            if(E_SET)
            {
                //easycon: pre-loaded duration
                SETWAIT(REG(_e_val));
                RESETAFTER(_keycode, 1);
            }
            else if((_ins1 & B10000000) == 0)
            {
                //easycon: standard
                n = _ins1 & B01111111;
                //easycon: unscale
                n *= 50;
                SETWAIT(n);
                RESETAFTER(_keycode, n);
            }
            else
            {
                //easycon: hold
                n = _ins1 & B01111111;
                RESETAFTER(_keycode, n);
            }
        }
    }
    else
    {
        //easycon: flow control
        switch((_ins0 >> 3) & B00001111)
        {
            case B00000000:
                if((_ins0 & B00000100) == 0)
                {
                    //easycon: empty
                }
                else
                {
                    //easycon: Instruction : SerialPrint
                    reg = _ins & ((1 << 9) - 1);
                    if((_ins0 & B00000010) == 0)
                    {
                        umain_send8B((uint8_t *)reg, 1);
                        umain_send8B((uint8_t *)(reg >> 8), 1);
                    }
                    else
                    {
                        umain_send8B((uint8_t *)mem[reg], 1);
                        umain_send8B((uint8_t *)mem[reg], 1);
                    }
                    break;
                }
                break;
            case B00000001:
                //easycon: Instruction : Wait
                if(E_SET)
                {
                    //easycon: pre-loaded duration
                    n = REG(_e_val);
                }
                else if((_ins0 & B00000100) == 0)
                {
                    //easycon: standard
                    n = _ins & ((1 << 10) - 1);
                    //easycon: unscale
                    n *= 10;
                }
                else if((_ins0 & B00000010) == 0)
                {
                    //easycon: extended
                    _ins2 = flash_ecScript_read8B((uint16_t)script_addr++);
                    _ins3 = flash_ecScript_read8B((uint16_t)script_addr++);
                    n = _insEx & ((1L << 25) - 1);
                    //easycon: unscale
                    n *= 10;
                }
                else
                {
                    //easycon: high precision
                    n = _ins & ((1 << 9) - 1);
                }
                SETWAIT(n);
                break;
            case B00000010:
                //easycon: Instruction : For
                if (_forstackindex == 0 || FOR_ADDR(_forstackindex - 1) != _addr)
                {
                    //easycon: loop initialize
                    _forstackindex++;
                    FOR_I(_forstackindex - 1) = 0;
                    FOR_ADDR(_forstackindex - 1) = _addr;
                    FOR_NEXT(_forstackindex - 1) = _ins & ((1 << 11) - 1);
                    //easycon: pre-loaded arguments
                    if(E_SET)
                    {
                        //easycon: store iterator
                        _ri0 = REG(_e_val) & 0xF;
                        if(_ri0 != 0)
                        {
                            //easycon: store iterator
                            FOR_I(_forstackindex - 1) = _ri0 | 0x80000000;
                        }
                        //easycon: count
                        _ri1 = (REG(_e_val) >> 4) & 0xF;
                        if(_ri1 != 0)
                        {
                            //easycon: write loop count
                            FOR_C(_forstackindex - 1) = REG(_ri1);
                            //easycon: Mode 2 : loop count overwritten
                            E(2);
                            //easycon: jump to next (for condition checking)
                            JUMP(FOR_NEXT(_forstackindex - 1));
                            break;
                        }
                    }
                    //easycon: Mode 0 : init
                    E(0);
                    //easycon: jump to next (for further initialization)
                    JUMP(FOR_NEXT(_forstackindex - 1));
                    break;
                }
                break;
            case B00000011:
                //easycon: Instruction : Next
                if (_ins0 & B00000100)
                {
                    //easycon: extended
                    _ins2 = flash_ecScript_read8B((uint16_t)script_addr++);
                    _ins3 = flash_ecScript_read8B((uint16_t)script_addr++);
                }
                if(E_SET)
                {
                    if (_e_val == 1)
                    {
                        //easycon: Mode 1 : break
                        _forstackindex--;
                        break;
                    }
                    else if(_e_val == 0)
                    {
                        //easycon: Mode 0 : init
                        //easycon: initialize loop count
                        if((_ins0 & B00000100) == 0)
                        {
                            //easycon: small number
                            FOR_C(_forstackindex - 1) = _ins & ((1 << 10) - 1);
                            if(FOR_C(_forstackindex - 1) == 0)
                            {
                                //easycon: infinite loop
                                FOR_C(_forstackindex - 1) = 0x80000000;
                            }
                        }
                        else
                        {
                            //easycon: large number
                            FOR_C(_forstackindex - 1) = _insEx & ((1L << 26) - 1);
                        }
                    }                
                    else if(_e_val == 2)
                    {
                        //easycon: Mode 2 : loop count overwritten
                        //easycon: do nothing here
                    }
                }
                else
                {
                    //easycon: normal loop step
                    if(FOR_I(_forstackindex - 1) & 0x80000000)
                    {
                        //easycon: iterator
                        REG(FOR_I(_forstackindex - 1) & 0xF) += 1;
                    }
                    else
                    {
                        //easycon: loop variable
                        FOR_I(_forstackindex - 1) += 1;
                    }
                }

                //easycon: check condition
                if (FOR_I(_forstackindex - 1) & 0x80000000)
                {
                    n = REG(FOR_I(_forstackindex - 1) & 0xF);
                }
                else
                {
                    n = FOR_I(_forstackindex - 1);
                }

                if(FOR_C(_forstackindex - 1) != 0x80000000 && n >= FOR_C(_forstackindex - 1))
                {
                    //easycon: end for
                    _forstackindex--;
                }
                else
                {
                    //easycon: jump back
                    JUMP(FOR_ADDR(_forstackindex - 1));
                }
                break;
            case B00000100:
                if(_ins0 & B00000100)
                {
                    //easycon: comparisons
                    _ri0 = (_ins1 >> 3) & B00000111;
                    _ri1 = _ins1 & B00000111;
                    switch(_ins0 & B00000011)
                    {
                        case B00000000:
                            //easycon: Instruction : Equal
                            _v = REG(_ri0) == REG(_ri1);
                            break;
                        case B00000001:
                            //easycon: Instruction : NotEqual
                            _v = REG(_ri0) != REG(_ri1);
                            break;
                        case B00000010:
                            //easycon: Instruction : LessThan
                            _v = REG(_ri0) < REG(_ri1);
                            break;
                        case B00000011:
                            //easycon: Instruction : LessOrEqual
                            _v = REG(_ri0) <= REG(_ri1);
                            break;
                    }
                    _v = (bool)_v;
                    _flag = (bool)_flag;
                    switch(_ins1 >> 6)
                    {
                        case B00000000:
                            //easycon: assign
                            _flag = _v;
                            break;
                        case B00000001:
                            //easycon: and
                            _flag &= _v;
                            break;
                        case B00000010:
                            //easycon: or
                            _flag |= _v;
                            break;
                        case B00000011:
                            //easycon: xor
                            _flag ^= _v;
                            break;
                    }
                }
                else
                {
                    switch(_ins1 >> 5)
                    {
                        case B00000000:
                            //easycon: Instruction : Break
                            if ((_ins1 & B00010000) && !_flag)
                            {
                                break;
                            }
                            _v = _ins1 & B00001111;
                            _forstackindex -= _v;
                            E(1);
                            JUMP(FOR_NEXT(_forstackindex - 1));
                            break;
                        case B00000001:
                            //easycon: Instruction : Continue
                            if ((_ins1 & B00010000) && !_flag)
                            {
                                break;
                            }
                            _v = _ins1 & B00001111;
                            _forstackindex -= _v;
                            JUMP(FOR_NEXT(_forstackindex - 1));
                            break;
                        case B00000111:
                            //easycon: Instruction : Return
                            if ((_ins1 & B00010000) && !_flag)
                            {
                                break;
                            }
                            if (_callstackindex)
                            {
                                //easycon: sub function
                                //easycon: pop return address
                                JUMP(CALLSTACK(_callstackindex - 1));
                                _callstackindex--;
                                break;
                            }
                            else
                            {
                                //easycon: main function
                                ecScript_stop();
                                break;
                            }
                    }
                }
                break;
            case B00000101:
                if ((_ins0 & B00000100) == 0)
                {
                    _ri0 = (_ins >> 7) & B00000111;
                    if(_ri0 == 0)
                    {
                        if((_ins1 & (1 << 6)) == 0)
                        {
                            //easycon: binary operations on instant
                            _ins2 = flash_ecScript_read8B((uint16_t)script_addr++);
                            _ins3 = flash_ecScript_read8B((uint16_t)script_addr++);
                            _v = (_ins >> 3) & B00000111;
                            _ri0 = _ins & B00000111;
                            reg = _insEx;
                            BinaryOp(_v, _ri0, reg);
                        }
                        else
                        {
                            //easycon: preserved
                        }
                    }
                    else
                    {
                        //easycon: Instruction : Mov
                        reg = _ins1 & B01111111;
                        //easycon: fill sign bit
                        reg <<= 9;
                        reg >>= 9;
                        REG(_ri0) = reg;
                    }
                }
                else if((_ins0 & B00000110) == B00000100)
                {
                    //easycon: binary operations on register
                    _v = (_ins >> 6) & B00000111;
                    _ri0 = (_ins >> 3) & B00000111;
                    _ri1 = _ins & B00000111;
                    BinaryOp(_v, _ri0, REG(_ri1));
                }
                else if((_ins0 & B00000111) == B00000110)
                {
                    //easycon: bitwise shift
                    _ri0 = (_ins1 >> 4) & B00000111;
                    _v = _ins1 & B00001111;
                    if(_ri0 == 0)
                    {
                        break;
                    }
                    if((_ins1 & B10000000) == 0)
                    {
                        //easycon: Instruction : ShL
                        REG(_ri0) <<= _v;
                    }
                    else
                    {
                        //easycon: Instruction : ShR
                        REG(_ri0) >>= _v;
                    }
                }
                else
                {
                    //easycon: unary operations
                    _ri0 = _ins1 & B00000111;
                    switch((_ins1 >> 3) & B00001111)
                    {
                        case B00000010:
                            //easycon: Instruction : Negative
                            if(_ri0 == 0)
                            {
                                break;
                            }
                            REG(_ri0) = -REG(_ri0);
                            break;
                        case B00000011:
                            //easycon: Instruction : Not
                            if (_ri0 == 0)
                            {
                                break;
                            }
                            REG(_ri0) = ~REG(_ri0);
                            break;
                        case B00000100:
                            //easycon: Instruction : Push
                            _stackindex++;
                            STACK(_stackindex - 1) = REG(_ri0);
                            break;
                        case B00000101:
                            //easycon: Instruction : Pop
                            if (_ri0 == 0)
                            {
                                break;
                            }
                            REG(_ri0) = STACK(_stackindex - 1);
                            _stackindex--;
                            break;
                        case B00000111:
                            //easycon: Instruction : StoreOp
                            E(_ri0);
                            break;
                        case B00001000:
                            //easycon: Instruction : Bool
                            if (_ri0 == 0)
                            {
                                break;
                            }
                            REG(_ri0) = (bool)REG(_ri0);
                            break;
                        case B00001001:
                        //easycon: Instruction : Rand
                            if (_ri0 == 0)
                            {
                                break;
                            }
                            if(!_seed)
                            {
                                _seed = timer_ms;
                                ec_seed = _seed;
                                flash_userVal_write();
                                srand(_seed);
                            }
                            REG(_ri0) = rand() % REG(_ri0);
                            break;
                    }
                }
                break;
            case B00000110:
                //easycon: branches
                reg = _ins & ((1 << 9) - 1);
                reg = reg << 7 >> 6;
                switch ((_ins0 >> 1) & B00000011)
                {
                    case B00000000:
                        //easycon: Instruction : Branch
                        JUMPNEAR(reg);
                        break;
                    case B00000001:
                        //easycon: Instruction : BranchTrue
                        if(_flag)
                        {
                            JUMPNEAR(reg);
                        }
                        break;
                    case B00000010:
                        //easycon: Instruction : BranchFalse
                        if (!_flag)
                        {
                            JUMPNEAR(reg);
                        }
                        break;
                    case B00000011:
                        //easycon: Instruction : Call
                        _callstackindex++;
                        STACK(_callstackindex - 1) = (int16_t)script_addr;
                        JUMPNEAR(reg);
                        break;
                }
                break;
        }
    }

    usbd_serialcon_input_handle(ecScript_Button, ecScript_HAT, ecScript_LX, ecScript_LY, ecScript_RX, ecScript_RY);
    return 1;
}

void ecScript_init(void)
{
    srand(ec_seed);
    flash_writeErase(userMemAddr_Start, 1);
    flash_write32B(userMemAddr_Start + ecVal_seed_Offset, (uint32_t)ec_seed);

    //easycon: calculate direction presets
    for (int i = 0; i < 16; i++)
    {
        //easycon: part 1
        int x = (Min(i, 8)) << 5;
        int y = (Max(i, 8) - 8) << 5;
        x = Min(x, 255);
        y = Min(y, 255);
        DX(i) = x;
        DY(i) = y;
    }
    for (int i = 16; i < 32; i++)
    {
        //easycon: part 2
        int x = (24 - Min(i, 24)) << 5;
        int y = (32 - Max(i, 24)) << 5;
        x = Min(x, 255);
        y = Min(y, 255);
        DX(i) = x;
        DY(i) = y;
    }
}

void ecScript_start(void)
{
    wait_ms = 0;
    timer_ms = 0;
    memset(mem + VARSPACE_OFFSET, 0, sizeof(mem) - VARSPACE_OFFSET);
    ecScript_Flag = 1;
    _seed = ec_seed;
}

void ecScript_stop(void)
{
    ecScript_Flag = 0;
    timer_elapsed = timer_ms;
    usbd_gp_init();
}

//easycon: Perform binary operations by operator code
void BinaryOp(uint8_t op, uint8_t reg, int16_t value)
{
    if (reg == 0)
        return;
    switch (op)
    {
        case B00000000:
            //easycon: Mov
            REG(reg) = value;
            break;
        case B00000001:
            //easycon: Add
            REG(reg) += value;
            break;
        case B00000010:
            //easycon: Mul
            REG(reg) *= value;
            break;
        case B00000011:
            //easycon: Div
            REG(reg) /= value;
            break;
        case B00000100:
            //easycon: Mod
            REG(reg) %= value;
            break;
        case B00000101:
            //easycon: And
            REG(reg) &= value;
            break;
        case B00000110:
            //easycon: Or
            REG(reg) |= value;
            break;
        case B00000111:
            //easycon: Xor
            REG(reg) ^= value;
            break;
    }
}
