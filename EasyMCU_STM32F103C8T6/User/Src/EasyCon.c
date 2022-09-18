#include "EasyCon.h"
//#include "EasyCon_API.h"

// global variables
static uint8_t mem[MEM_SIZE] = {0xFF, 0xFF, VERSION}; // preallocated memory for all purposes, as well as static instruction carrier

// static variables
static size_t serial_buffer_length = 0;               // current length of serial buffer
static bool serial_command_ready = false;             // CMD_READY acknowledged, ready to receive command byte
static uint8_t *flash_addr = 0;                       // start location for EEPROM flashing
static uint16_t flash_index = 0;                      // current buffer index
static uint16_t flash_count = 0;                      // number of bytes expected for this time
static uint8_t *script_addr = 0;                      // address of next instruction
static uint8_t *script_eof = 0;                       // address of EOF
static uint16_t tail_wait = 0;                        // insert an extra wait before next instruction (used by compressed instruction)
static uint32_t timer_elapsed = 0;                    // previous execution time
static bool auto_run = false;
static volatile uint8_t echo_ms = 0; // echo counter

// set led state
static volatile uint8_t _ledflag = 0;

// timers define
static volatile uint32_t timer_ms = 0; // script timer
static volatile uint32_t wait_ms = 0;  // waiting counter

// some funcs only use in EasyCon
static void binaryop(uint8_t op, uint8_t reg, int16_t value);

// Initialize script. Load static script into EEPROM if exists.
void EasyCon_script_init(void)
{
    uint16_t len = 0;
    if ((mem[0] != 0xFF) || (mem[1] != 0xFF))
    {
        len = mem[0] | ((mem[1] & 0x7F) << 8);
        // flash instructions from firmware
        EasyCon_write_start(0);
        EasyCon_write_data((uint8_t *)0,mem,len);
        // for saving flash times
			  _seed = (uint16_t)EasyCon_read_byte((uint8_t *)SEED_OFFSET+1)<<8 | EasyCon_read_byte((uint8_t *)SEED_OFFSET);
				_seed +=1;
        srand(_seed);
				EasyCon_write_data((uint8_t *)SEED_OFFSET,(uint8_t*)&_seed,2);
        EasyCon_write_end(0);
    }
    else
    {
        // randomize
			  _seed = (uint16_t)EasyCon_read_byte((uint8_t *)SEED_OFFSET+1)<<8 | EasyCon_read_byte((uint8_t *)SEED_OFFSET);
				_seed +=1;
        srand(_seed);
        EasyCon_write_start(1);
				EasyCon_write_data((uint8_t *)SEED_OFFSET,(uint8_t*)&_seed,2);
        EasyCon_write_end(1);
    }
    memset(mem, 0, sizeof(mem));

    // calculate direction presets
    for (int i = 0; i < 16; i++)
    {
        // part 1
        int x = (Min(i, 8)) << 5;
        int y = (Max(i, 8) - 8) << 5;
        x = Min(x, 255);
        y = Min(y, 255);
        DX(i) = x;
        DY(i) = y;
    }
    for (int i = 16; i < 32; i++)
    {
        // part 2
        int x = (24 - Min(i, 24)) << 5;
        int y = (32 - Max(i, 24)) << 5;
        x = Min(x, 255);
        y = Min(y, 255);
        DX(i) = x;
        DY(i) = y;
    }
    _report_echo = ECHO_TIMES;
    // turn on/off led
    _ledflag = EasyCon_read_byte((uint8_t *)LED_SETTING);
    // only if highest bit is 0
    auto_run = (EasyCon_read_byte((uint8_t *)1) >> 7) == 0;
}

void EasyCon_script_tick(void)
{
    // decrement waiting counter
    if (wait_ms != 0 && (_report_echo == 0 || wait_ms > 1))
        wait_ms--;
}

void EasyCon_tick(void)
{
    // increment timer
    timer_ms++;
    // decrement echo counter
    if (echo_ms != 0)
        echo_ms--;
    EasyCon_script_tick();
}

void EasyCon_report_send_callback(void)
{
    // decrement echo counter
    if (!_script_running || _report_echo > 0 || wait_ms < 2)
    {
        _report_echo = Max(0, _report_echo - 1);
    }
    echo_ms = ECHO_INTERVAL;
}

bool EasyCon_is_script_running(void)
{
    if(_script_running == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Run script on startup.
void EasyCon_script_auto_start(void)
{
    if (auto_run)
        EasyCon_script_start();
}

// Run script.
void EasyCon_script_start(void)
{
    script_addr = (uint8_t *)2;
    uint16_t eof = EasyCon_read_byte((uint8_t *)0) | (EasyCon_read_byte((uint8_t *)1) << 8);
    if (eof == 0xFFFF)
        eof = 0;
    script_eof = (uint8_t *)(eof & 0x7FFF);
    // reset variables
    wait_ms = 0;
    ///////////////////////////
    echo_ms = 0;
    ///////////////////////////
    timer_ms = 0;
    tail_wait = 0;
    memset(mem + VARSPACE_OFFSET, 0, sizeof(mem) - VARSPACE_OFFSET);
    _script_running = 1;
    _seed = (uint16_t)EasyCon_read_byte((uint8_t *)SEED_OFFSET+1)<<8 | EasyCon_read_byte((uint8_t *)SEED_OFFSET);

    if(_ledflag != 0) return;
    EasyCon_runningLED_on();
}

// Stop script.
void EasyCon_script_stop(void)
{
    _script_running = 0;
    timer_elapsed = timer_ms;
    ////////////////////////////
    reset_hid_report();
    ///////////////////////////
    _report_echo = ECHO_TIMES;

    EasyCon_runningLED_off();
}

// Process script instructions.
void EasyCon_script_task(void)
{
    while (true)
    {
        // status check
        if (!_script_running)
            return;
        // timer check
        if (wait_ms > 0)
            return;
        // release keys
        EasyCon_blink_led();
        for (int i = 0; i <= KEYCODE_MAX; i++)
        {
            if (KEY(i) != 0)
            {
                KEY(i)--;
                if (KEY(i) == 0)
                {
                    if (i == 32)
                    {
                        // LS
                        set_left_stick(STICK_CENTER, STICK_CENTER);
                        _report_echo = ECHO_TIMES;
                    }
                    else if (i == 33)
                    {
                        // RS
                        set_right_stick(STICK_CENTER, STICK_CENTER);
                        _report_echo = ECHO_TIMES;
                    }
                    else if ((i & 0x10) == 0)
                    {
                        // Button
                        release_buttons(_BV(i));
                        _report_echo = ECHO_TIMES;
                    }
                    else
                    {
                        // HAT
                        set_HAT_switch(HAT_CENTER);
                        _report_echo = ECHO_TIMES;
                    }
                }
            }
        }
        if (tail_wait != 0)
        {
            // wait after compressed instruction
            SETWAIT(tail_wait);
            tail_wait = 0;
            return;
        }
        if (script_addr >= script_eof)
        {
            // reaches EOF, end script
            EasyCon_script_stop();
            return;
        }
        _addr = (uint16_t)script_addr;
        _ins0 = EasyCon_read_byte(script_addr++);
        _ins1 = EasyCon_read_byte(script_addr++);
        int32_t n;
        int16_t reg;
				// 0b10000000
        if (_ins0 & 0x80)
        {
            // key/stick actions
						// 0b01000000
            if ((_ins0 & 0x40) == 0)
            {
                // Instruction : Key 0b11111
                _keycode = (_ins0 >> 1) & 0x1F;
                // modify report
                if ((_keycode & 0x10) == 0)
                {
                    // Button
                    press_buttons(_BV(_keycode));
                    _report_echo = ECHO_TIMES;
                }
                else
                {
                    // HAT
                    set_HAT_switch(_keycode & 0xF);
                    _report_echo = ECHO_TIMES;
                }
                // post effect
                if (E_SET)
                {
                    // pre-loaded duration
                    SETWAIT(REG(_e_val));
                    RESETAFTER(_keycode, 1);
                }
								// 0b00000001
                else if ((_ins0 & 0x01) == 0)
                {
                    // standard
                    n = _ins1;
                    // unscale
                    n *= 10;
                    SETWAIT(n);
                    RESETAFTER(_keycode, 1);
                }
								// 0b10000000
                else if ((_ins1 & 0x80) == 0)
                {
                    // compressed 0b01111111
                    tail_wait = _ins1 & 0x7F;
                    // unscale
                    tail_wait *= 50;
                    SETWAIT(50);
                    RESETAFTER(_keycode, 1);
                }
                else
                {
                    // hold 0b01111111
                    n = _ins1 & 0x7F;
                    RESETAFTER(_keycode, n);
                }
            }
            else
            {
                // Instruction : Stick
                _lr = (_ins0 >> 5) & 1;
                _keycode = 32 | _lr;
								// 0b11111
                _direction = _ins0 & 0x1F;
                // modify report
                if (_lr)
                {
                    // RS
                    set_right_stick(DX(_direction), DY(_direction));
                    _report_echo = ECHO_TIMES;
                }
                else
                {
                    // LS
                    set_left_stick( DX(_direction), DY(_direction));
                    _report_echo = ECHO_TIMES;
                }
                // post effect
                if (E_SET)
                {
                    // pre-loaded duration
                    SETWAIT(REG(_e_val));
                    RESETAFTER(_keycode, 1);
                }
								// 0b10000000
                else if ((_ins1 & 0x80) == 0)
                {
                    // standard 0b01111111
                    n = _ins1 & 0x7F;
                    // unscale
                    n *= 50;
                    SETWAIT(n);
                    RESETAFTER(_keycode, 1);
                }
                else
                {
                    // hold 0b01111111
                    n = _ins1 & 0x7F;
                    RESETAFTER(_keycode, n);
                }
            }
        }
        else
        {
            // flow control 0b1111
            switch ((_ins0 >> 3) & 0x0F)
            {
						// 0b0000
            case 0x00:
								// 0b100
                if ((_ins0 & 0x04) == 0)
                {
                    // empty
                }
                else
                {
                    // Instruction : SerialPrint
                    reg = _ins & ((1 << 9) - 1);
										// 0b10
                    if ((_ins0 & 0x02) == 0)
                    {
                        EasyCon_serial_send(reg);
                        EasyCon_serial_send(reg >> 8);
                    }
                    else
                    {
                        EasyCon_serial_send(mem[reg]);
                        EasyCon_serial_send(mem[reg + 1]);
                    }
                    break;
                }
                break;
						// 0b0001
            case 0x01:
                // Instruction : Wait
                if (E_SET)
                {
                    // pre-loaded duration
                    n = REG(_e_val);
                }
								// 0b100
                else if ((_ins0 & 0x04) == 0)
                {
                    // standard
                    n = _ins & ((1 << 10) - 1);
                    // unscale
                    n *= 10;
                }
								// 0b10
                else if ((_ins0 & 0x02) == 0)
                {
                    // extended
                    _ins2 = EasyCon_read_byte(script_addr++);
                    _ins3 = EasyCon_read_byte(script_addr++);
                    n = _insEx & ((1L << 25) - 1);
                    // unscale
                    n *= 10;
                }
                else
                {
                    // high precision
                    n = _ins & ((1 << 9) - 1);
                }
                SETWAIT(n);
                break;
						// 0b0010
            case 0x02:
                // Instruction : For
                if (_forstackindex == 0 || FOR_ADDR(_forstackindex - 1) != _addr)
                {
                    // loop initialize
                    _forstackindex++;
                    FOR_I(_forstackindex - 1) = 0;
                    FOR_ADDR(_forstackindex - 1) = _addr;
                    FOR_NEXT(_forstackindex - 1) = _ins & ((1 << 11) - 1);
                    // pre-loaded arguments
                    if (E_SET)
                    {
                        // iterator
                        _ri0 = REG(_e_val) & 0xF;
                        if (_ri0 != 0)
                        {
                            // store iterator
                            FOR_I(_forstackindex - 1) = _ri0 | 0x80000000;
                        }
                        // count
                        _ri1 = (REG(_e_val) >> 4) & 0xF;
                        if (_ri1 != 0)
                        {
                            // write loop count
                            FOR_C(_forstackindex - 1) = REG(_ri1);
                            // Mode 2 : loop count overwritten
                            E(2);
                            // jump to next (for condition checking)
                            JUMP(FOR_NEXT(_forstackindex - 1));
                            break;
                        }
                    }
                    // Mode 0 : init
                    E(0);
                    // jump to next (for further initialization)
                    JUMP(FOR_NEXT(_forstackindex - 1));
                    break;
                }
                break;
						// 0b0011
            case 0x03:
                // Instruction : Next 0b100
                if (_ins0 & 0x04)
                {
                    // extended
                    _ins2 = EasyCon_read_byte(script_addr++);
                    _ins3 = EasyCon_read_byte(script_addr++);
                }
                if (E_SET)
                {
                    if (_e_val == 1)
                    {
                        // Mode 1 : break
                        _forstackindex--;
                        break;
                    }
                    else if (_e_val == 0)
                    {
                        // Mode 0 : init
                        if (_e_val == 0)
                        {
                            // initialize loop count 0b100
                            if ((_ins0 & 0x04) == 0)
                            {
                                // small number
                                FOR_C(_forstackindex - 1) = _ins & ((1 << 10) - 1);
                                if (FOR_C(_forstackindex - 1) == 0)
                                {
                                    // infinite loop
                                    FOR_C(_forstackindex - 1) = 0x80000000;
                                }
                            }
                            else
                            {
                                // large number
                                FOR_C(_forstackindex - 1) = _insEx & ((1L << 26) - 1);
                            }
                        }
                    }
                    else if (_e_val == 2)
                    {
                        // Mode 2 : loop count overwritten
                        // do nothing here
                    }
                }
                else
                {
                    // normal loop step
                    if (FOR_I(_forstackindex - 1) & 0x80000000)
                    {
                        // iterator
                        REG(FOR_I(_forstackindex - 1) & 0xF) += 1;
                    }
                    else
                    {
                        // loop variable
                        FOR_I(_forstackindex - 1) += 1;
                    }
                }
                // check condition
                if (FOR_I(_forstackindex - 1) & 0x80000000)
                    n = REG(FOR_I(_forstackindex - 1) & 0xF);
                else
                    n = FOR_I(_forstackindex - 1);
                if (FOR_C(_forstackindex - 1) != 0x80000000 && n >= FOR_C(_forstackindex - 1))
                {
                    // end for
                    _forstackindex--;
                }
                else
                {
                    // jump back
                    JUMP(FOR_ADDR(_forstackindex - 1));
                }
                break;
						// 0b0100
            case 0x04:
								// 0b100
                if (_ins0 & 0x04)
                {
                    // comparisons 0b111
                    _ri0 = (_ins1 >> 3) & 0x07;
										// 0b111
                    _ri1 = _ins1 & 0x07;
										// 0b11
                    switch (_ins0 & 0x03)
                    {
										// 0b00
                    case 0x00:
                        // Instruction : Equal
                        _v = REG(_ri0) == REG(_ri1);
                        break;
										// 0b01
                    case 0x01:
                        // Instruction : NotEqual
                        _v = REG(_ri0) != REG(_ri1);
                        break;
										// 0b10
                    case 0x02:
                        // Instruction : LessThan
                        _v = REG(_ri0) < REG(_ri1);
                        break;
										// 0b11
                    case 0x03:
                        // Instruction : LessOrEqual
                        _v = REG(_ri0) <= REG(_ri1);
                        break;
                    }
                    _v = (bool)_v;
                    _flag = (bool)_flag;
                    switch (_ins1 >> 6)
                    {
										// 0b00
                    case 0x00:
                        // assign
                        _flag = _v;
                        break;
										// 0b01
                    case 0x01:
                        // and
                        _flag &= _v;
                        break;
										// 0b10
                    case 0x02:
                        // or
                        _flag |= _v;
                        break;
										// 0b11
                    case 0x03:
                        // xor
                        _flag ^= _v;
                        break;
                    }
                }
                else
                {
                    switch (_ins1 >> 5)
                    {
										// 0b000
                    case 0x00:
                        // Instruction : Break 0b10000
                        if ((_ins1 & 0x10) && !_flag)
                            break;
												// 0b1111
                        _v = _ins1 & 0x0F;
                        _forstackindex -= _v;
                        E(1);
                        JUMP(FOR_NEXT(_forstackindex - 1));
                        break;
										// 0b001
                    case 0x01:
                        // Instruction : Continue 0b10000
                        if ((_ins1 & 0x10) && !_flag)
                            break;
												// 0b1111
                        _v = _ins1 & 0x0F;
                        _forstackindex -= _v;
                        JUMP(FOR_NEXT(_forstackindex - 1));
                        break;
										// 0b111
                    case 0x07:
                        // Instruction : Return 0b10000
                        if ((_ins1 & 0x10) && !_flag)
                            break;
                        if (_callstackindex)
                        {
                            // sub function
                            // pop return address
                            JUMP(CALLSTACK(_callstackindex - 1));
                            _callstackindex--;
                        }
                        else
                        {
                            // main function
                            EasyCon_script_stop();
                        }
                        break;
                    }
                }
                break;
						// 0b0101
            case 0x05:
								// 0b100
                if ((_ins0 & 0x04) == 0)
                {
										// 0b111
                    _ri0 = (_ins >> 7) & 0x07;
                    if (_ri0 == 0)
                    {
                        if ((_ins1 & (1 << 6)) == 0)
                        {
                            // binary operations on instant
                            _ins2 = EasyCon_read_byte(script_addr++);
                            _ins3 = EasyCon_read_byte(script_addr++);
														// 0b111
                            _v = (_ins >> 3) & 0x07;
                            _ri0 = _ins & 0x07;
                            reg = _insEx;
                            binaryop(_v, _ri0, reg);
                        }
                        else
                        {
                            // preserved
                        }
                    }
                    else
                    {
                        // Instruction : Mov 0b01111111
                        reg = _ins1 & 0x7F;
                        // fill sign bit
                        reg <<= 9;
                        reg >>= 9;
                        REG(_ri0) = reg;
                    }
                }
								// 0b110 0b100
                else if ((_ins0 & 0x06) == 0x04)
                {
                    // binary operations on register
                    _v = (_ins >> 6) & 0x07;
                    _ri0 = (_ins >> 3) & 0x07;
                    _ri1 = _ins & 0x07;
                    binaryop(_v, _ri0, REG(_ri1));
                }
								// 0b111 0b110
                else if ((_ins0 & 0x07) == 0x06)
                {
                    // bitwise shift
                    _ri0 = (_ins1 >> 4) & 0x07;
                    _v = _ins1 & 0x0F;
                    if (_ri0 == 0)
                        break;
                    if ((_ins1 & 0x80) == 0)
                    {
                        // Instruction : ShL
                        REG(_ri0) <<= _v;
                    }
                    else
                    {
                        // Instruction : ShR
                        REG(_ri0) >>= _v;
                    }
                }
                else
                {
                    // unary operations
                    _ri0 = _ins1 & 0x07;
                    switch ((_ins1 >> 3) & 0x0F)
                    {
										// 0b0010
                    case 0x02:
                        // Instruction : Negative
                        if (_ri0 == 0)
                            break;
                        REG(_ri0) = -REG(_ri0);
                        break;
										// 0b0011
                    case 0x03:
                        // Instruction : Not
                        if (_ri0 == 0)
                            break;
                        REG(_ri0) = ~REG(_ri0);
                        break;
										// 0b0100
                    case 0x04:
                        // Instruction : Push
                        _stackindex++;
                        STACK(_stackindex - 1) = REG(_ri0);
                        break;
									  // 0b0101
                    case 0x05:
                        // Instruction : Pop
                        if (_ri0 == 0)
                            break;
                        REG(_ri0) = STACK(_stackindex - 1);
                        _stackindex--;
                        break;
										// 0b0111
                    case 0x07:
                        // Instruction : StoreOp
                        E(_ri0);
                        break;
										// 0b1000
                    case 0x08:
                        // Instruction : Bool
                        if (_ri0 == 0)
                            break;
                        REG(_ri0) = (bool)REG(_ri0);
                        break;
										// 0b1001
                    case 0x09:
                        // Instruction : Rand
                        if (_ri0 == 0)
                            break;
                        if (!_seed)
                        {
                            _seed = timer_ms;
                            EasyCon_write_start(1);
														EasyCon_write_data((uint8_t *)SEED_OFFSET,(uint8_t*)&_seed,2);
                            EasyCon_write_end(1);
                            srand(_seed);
                        }
                        REG(_ri0) = rand() % REG(_ri0);
                        break;
                    }
                }
                break;
						// 0b0110
            case 0x06:
                // branches
                reg = _ins & ((1 << 9) - 1);
                reg = reg << 7 >> 6;
                switch ((_ins0 >> 1) & 0x3)
                {
								// 0b00
                case 0x00:
                    // Instruction : Branch
                    JUMPNEAR(reg);
                    break;
								// 0b01
                case 0x01:
                    // Instruction : BranchTrue
                    if (_flag)
                        JUMPNEAR(reg);
                    break;
								// 0b10
                case 0x02:
                    // Instruction : BranchFalse
                    if (!_flag)
                        JUMPNEAR(reg);
                    break;
								// 0b11
                case 0x03:
                    // Instruction : Call
                    _callstackindex++;
                    STACK(_callstackindex - 1) = (int16_t)script_addr;
                    JUMPNEAR(reg);
                    break;
                }
                break;
            }
        }
    }
}

// Perform binary operations by operator code
static void binaryop(uint8_t op, uint8_t reg, int16_t value)
{
    if (reg == 0)
        return;
    switch (op)
    {
		// 0b000
    case 0x00:
        // Mov
        REG(reg) = value;
        break;
		// 0b001
    case 0x01:
        // Add
        REG(reg) += value;
        break;
		// 0b010
    case 0x02:
        // Mul
        REG(reg) *= value;
        break;
		// 0b011
    case 0x03:
        // Div
        REG(reg) /= value;
        break;
		// 0b100
    case 0x04:
        // Mod
        REG(reg) %= value;
        break;
		// 0b101
    case 0x05:
        // And
        REG(reg) &= value;
        break;
		// 0b110
    case 0x06:
        // Or
        REG(reg) |= value;
        break;
	  // 0b111
    case 0x07:
        // Xor
        REG(reg) ^= value;
        break;
    }
}

// Process data from serial port.
void EasyCon_serial_task(int16_t byte)
{
    if (byte < 0)
        return;
    EasyCon_blink_led();
    if (flash_index < flash_count)
    {
        // flashing
        SERIAL_BUFFER(flash_index) = byte;
        flash_index++;
        if (flash_index == flash_count)
        {
            // all bytes received,save them
            EasyCon_write_start(2);
            EasyCon_write_data(flash_addr,mem,flash_count);
            EasyCon_serial_send(REPLY_FLASHEND);
        }
    }
    else
    {
        // regular
        SERIAL_BUFFER(serial_buffer_length++) = byte;
        // check control byte
        if ((byte & 0x80) != 0)
        {
            if (serial_buffer_length == 1 && !serial_command_ready && byte == CMD_READY)
            {
                // comand ready
                serial_command_ready = true;
            }
            else if (serial_buffer_length == 8)
            {
                // report data
                if (_script_running)
                {
                    // script running, send BUSY
                    EasyCon_serial_send(REPLY_BUSY);
                }
                else
                {
                    //memset(&next_report, 0, sizeof(USB_JoystickReport_Input_t));
                    set_buttons( (SERIAL_BUFFER(0) << 9) | (SERIAL_BUFFER(1) << 2) | (SERIAL_BUFFER(2) >> 5) );
                    set_HAT_switch( (uint8_t)((SERIAL_BUFFER(2) << 3) | (SERIAL_BUFFER(3) >> 4)) );
                    set_left_stick( (uint8_t)((SERIAL_BUFFER(3) << 4) | (SERIAL_BUFFER(4) >> 3)),
                                    (uint8_t)((SERIAL_BUFFER(4) << 5) | (SERIAL_BUFFER(5) >> 2)));
                    set_right_stick( (uint8_t)((SERIAL_BUFFER(5) << 6) | (SERIAL_BUFFER(6) >> 1)),
                                    (uint8_t)((SERIAL_BUFFER(6) << 7) | (SERIAL_BUFFER(7) & 0x7f)));
                    // set flag
                    _report_echo = ECHO_TIMES;
                    // send ACK
                    EasyCon_serial_send(REPLY_ACK);
                }
                serial_command_ready = false;
            }
            else if (serial_command_ready)
            {
                serial_command_ready = false;
                // command
                switch (byte)
                {
                case CMD_DEBUG:;
                    uint32_t n;
                    // instruction count
                    //uint8_t* count = (uint8_t*)(eeprom_read_byte((uint8_t*)0) | (eeprom_read_byte((uint8_t*)1) << 8));
                    //for (uint8_t* i = 0; i < count; i++)
                    //EasyCon_serial_send(eeprom_read_byte(i));

                    // current loop variable
                    n = FOR_I(_forstackindex - 1);
                    for (int i = 0; i < 4; i++)
                    {
                        EasyCon_serial_send(n);
                        n >>= 8;
                    }

                    // time elapsed
                    n = timer_elapsed;
                    for (int i = 0; i < 4; i++)
                    {
                        EasyCon_serial_send(n);
                        n >>= 8;
                    }

                    // PC
                    n = (uint16_t)script_addr;
                    for (int i = 0; i < 2; i++)
                    {
                        EasyCon_serial_send(n);
                        n >>= 8;
                    }
                    break;
                case CMD_VERSION:
                    EasyCon_serial_send(VERSION);
                    break;
                case CMD_LED:
                    _ledflag ^= 0x8;
                    // there is no end , for flash one byte force
                    EasyCon_write_start(1);
										EasyCon_write_data((uint8_t *)LED_SETTING,(uint8_t*)&_ledflag,1);
                    EasyCon_write_end(1);
                    EasyCon_runningLED_off();
                    EasyCon_serial_send(_ledflag);
                    break;
                case CMD_READY:
                    serial_command_ready = true;
                    break;
                case CMD_HELLO:
                    EasyCon_serial_send(REPLY_HELLO);
                    break;
                case CMD_FLASH:
                    if (serial_buffer_length != 5)
                    {
                        EasyCon_serial_send(REPLY_ERROR);
                        break;
                    }
                    EasyCon_script_stop();
                    flash_addr = (uint8_t *)(SERIAL_BUFFER(0) | (SERIAL_BUFFER(1) << 7));
                    flash_count = (SERIAL_BUFFER(2) | (SERIAL_BUFFER(3) << 7));
                    flash_index = 0;
                    EasyCon_serial_send(REPLY_FLASHSTART);
                    break;
                case CMD_SCRIPTSTART:
                    EasyCon_script_start();
                    EasyCon_serial_send(REPLY_SCRIPTACK);
                    break;
                case CMD_SCRIPTSTOP:
                    EasyCon_script_stop();
                    EasyCon_serial_send(REPLY_SCRIPTACK);
                    break;
                default:
                    // error
                    EasyCon_serial_send(REPLY_ERROR);
                    break;
                }
								if(byte!=CMD_FLASH)
								{
									// lock flash
									EasyCon_write_end(0);
								}
            }
            else
            {
                // error
                EasyCon_serial_send(serial_buffer_length);
            }
            // clear buffer
            serial_buffer_length = 0;
        }
        // overflow protection
        if (serial_buffer_length >= SERIAL_BUFFER_SIZE)
            serial_buffer_length = 0;
    }
}

uint8_t EasyCon_is_LED_enable(void)
{
    return _ledflag;
}

/* check if need send report.
 * need implement
 */
bool EasyCon_need_send_report(void)
{
    if(echo_ms==0)
    {
        return true;
    }
    return false;
}
