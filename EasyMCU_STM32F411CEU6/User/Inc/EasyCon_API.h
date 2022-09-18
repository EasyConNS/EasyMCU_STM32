#ifndef _EASY_CON_API_H_
#define _EASY_CON_API_H_

/**********************************************************************/
// some incude files for var type, you need change to your device framework files
/**********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "HID.h"

/**********************************************************************/
// EasyCon API, you need set the MEM_SIZE that could use in your SRAM and (EEPROM or Flash)
// EEPROM or Flash need 4 bytes more than MEM_SIZE, so make sure you have enough space for it
/**********************************************************************/
#define MEM_SIZE      924
// EEPROM or Flash Size = MEM_SIZE + 4 
/**********************************************************************/
// EasyCon API, you need to call them in somewhere
/**********************************************************************/

/* the main tick for 1 ms
 * need call in a 1 ms timer
 */
extern void EasyCon_tick(void);

/* serial state machine
 * need call when get a new serial date from uart
 * no date return -1
 */
extern void EasyCon_serial_task(int16_t byte);

/* check if need send report.
 * need call in hid send task
 */
extern bool EasyCon_need_send_report(void);

/* need call when a report sent
 * 
 */
extern void EasyCon_report_send_callback(void);

/**********************************************************************/
// EasyCon API, you need to implement all of the API
/**********************************************************************/

/* EasyCon read 1 byte from E2Prom or flash 
 * need implement
 */
extern uint8_t EasyCon_read_byte(uint8_t* addr);

/* EasyCon write n byte to E2Prom or flash 
 * need implement
 */
extern void EasyCon_write_data(uint8_t* addr,uint8_t* data,uint16_t len);

/* EasyCon start write to E2Prom or flash callback
 * optional implement
 */
extern void EasyCon_write_start(uint8_t mode);

/* EasyCon write to E2Prom or flash end callback 
 * optional implement
 */
extern void EasyCon_write_end(uint8_t mode);

/* running led on
 * need implement
 */
extern void EasyCon_runningLED_on(void);

/* running led off
 * need implement
 */
extern void EasyCon_runningLED_off(void);

/* data led blink
 * need implement,no block
 */
extern void EasyCon_blink_led(void);

/* serial send 1 byte
 * need implement,block
 */
extern void EasyCon_serial_send(const char DataByte);

// about hid report

/* reset hid report to default.
 * need implement
 */
extern void reset_hid_report(void);

/* set left stick in hid report.
 * need implement
 */
extern void set_left_stick(const uint8_t LX, const uint8_t LY);

/* set right stick in hid report.
 * need implement
 */
extern void set_right_stick(const uint8_t RX, const uint8_t RY);

/* set button in hid report.
 * need implement
 */
extern void set_buttons(const uint16_t Button);

/* set button press in hid report.
 * need implement
 */
extern void press_buttons(const uint16_t Button);

/* set buttons release in hid report.
 * need implement
 */
extern void release_buttons(const uint16_t Button);

/* set HAT in hid report.
 * need implement
 */
extern void set_HAT_switch(const uint8_t HAT);

/**********************************************************************/
// here is some func that could control EasyCon, no need implement
/**********************************************************************/

extern void EasyCon_script_init(void);
extern void EasyCon_script_task(void);
extern void EasyCon_script_auto_start(void);
extern bool EasyCon_is_script_running(void);
extern void EasyCon_script_start(void);
extern void EasyCon_script_stop(void);

#endif
