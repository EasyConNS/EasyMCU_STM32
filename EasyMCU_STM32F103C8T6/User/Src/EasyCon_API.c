#include "EasyCon_API.h"

/**********************************************************************/
// some incude files for funcs, you need change to your device framework files
/**********************************************************************/
#include "flash.h"
#include "led.h"

/**********************************************************************/
// EasyCon API, you need to implement all of the AIP
/**********************************************************************/

/* EasyCon read 1 byte from E2Prom or flash 
 * need implement
 */
uint8_t EasyCon_read_byte(uint8_t* addr)
{
    return flash_ecScript_read8B((uint16_t)addr);
}

/* EasyCon write n byte to E2Prom or flash 
 * need implement
 */
void EasyCon_write_data(uint8_t* addr,uint8_t* data,uint16_t len)
{
    flash_ecScript_write8B(data,len,(uint16_t)addr);
}

/* EasyCon start write to E2Prom or flash callback
 * optional implement
 * flash script mode:0 could clean all
 * flash config mode:1 need save other flash data
 */
void EasyCon_write_start(uint8_t mode)
{
    uint8_t * p_buffer;
    // no need save data
    if(mode == 0)
    {
        flash_writeUnlock();
        flash_writeErase(ecMemAddr_Start, 1);
    }

    // need save data
    if(mode == 1)
    {
        flash_writeUnlock();
        // save all data
        p_buffer = (uint8_t *)malloc(MEM_SIZE);
        flash_ecScript_read(p_buffer,MEM_SIZE,0);
        // clean
        flash_writeErase(ecMemAddr_Start, 1);
        // recover it
        flash_ecScript_write8B(p_buffer,MEM_SIZE,0);
        free(p_buffer);
    }
		
		if(mode == 2)
		{
			// need clean when first time call
			if(flash_writeUnlock()==0)
			{
				flash_writeErase(ecMemAddr_Start, 1);
			}
		}
}

/* EasyCon write to E2Prom or flash end callback 
 * optional implement
 * flash script mode:0
 * flash config mode:1 
 */
void EasyCon_write_end(uint8_t mode)
{
    flash_writeLock();
}

/* running led on
 * need implement
 */
void EasyCon_runningLED_on(void)
{
	ledb_on();
}

/* running led off
 * need implement
 */
void EasyCon_runningLED_off(void)
{
    ledb_off();
}

/* data led blink
 * need implement,no block
 */
void EasyCon_blink_led(void)
{
    // no blink led
}

/* serial send 1 byte
 * need implement,block
 */
void EasyCon_serial_send(const char DataByte)
{
    umain_send8B((uint8_t *)&DataByte, 1);
    EasyCon_blink_led();
}

/* reset hid report to default.
 * need implement
 */
void reset_hid_report(void)
{
    ResetReport();
}

/* set left stick in hid report.
 * need implement
 */
void set_left_stick(const uint8_t LX, const uint8_t LY)
{
    SetLeftStick(LX,LY);
}

/* set right stick in hid report.
 * need implement
 */
void set_right_stick(const uint8_t RX, const uint8_t RY)
{
    SetRightStick(RX,RY);
}

/* set button in hid report.
 * need implement
 */
void set_buttons(const uint16_t Button)
{
    SetButtons(Button);
}

/* set button press in hid report.
 * need implement
 */
void press_buttons(const uint16_t Button)
{
    PressButtons(Button);
}

/* set buttons release in hid report.
 * need implement
 */
void release_buttons(const uint16_t Button)
{
    ReleaseButtons(Button);
}

/* set HAT in hid report.
 * need implement
 */
void set_HAT_switch(const uint8_t HAT)
{
    SetHATSwitch(HAT);
}
