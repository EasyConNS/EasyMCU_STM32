#include "HID.h"
#include "usbd_customhid.h" 
extern USBD_HandleTypeDef hUsbDeviceFS; 

USB_JoystickReport_Input_t next_report;
char *TAG_USBD = "[USBD]";
char *TAG_SEND_BUF = "[SB]";
char *TAG_SEND_RAW_BUF = "[SWB]";
char *TAG_SEND_INIT_BUF = "[SIB]";

// Reset report to default.
void ResetReport(void)
{
  memset((void *)&next_report, 0, sizeof(USB_JoystickReport_Input_t));
  next_report.LX = STICK_CENTER;
  next_report.LY = STICK_CENTER;
  next_report.RX = STICK_CENTER;
  next_report.RY = STICK_CENTER;
  next_report.HAT = HAT_CENTER;
}
void SetButtons(const uint16_t Button) {next_report.Button = Button;}
void PressButtons(const uint16_t Button) {next_report.Button |= Button;}
void ReleaseButtons(const uint16_t Button) {next_report.Button &= ~(Button);}
void SetHATSwitch(const uint8_t HAT) {next_report.HAT = HAT;}
void SetLeftStick(const uint8_t LX, const uint8_t LY)
{
  next_report.LX = LX; next_report.LY = LY;
}
void SetRightStick(const uint8_t RX, const uint8_t RY)
{
  next_report.RX = RX; next_report.RY = RY;
}


/* NNK: 日志打印函数 */
static void usbd_send_log(uint8_t *send_buf, char *TAG)
{
	printf("%s %s:", TAG_USBD, TAG);
	printx("%02x %02x %02x %02x %02x %02x %02x %02x", send_buf[0], send_buf[1], send_buf[2], send_buf[3], send_buf[4], send_buf[5], send_buf[6], send_buf[7]);
}

/* NNK: 底层控制函数 */
/* NNK: USBD发送数组: usbd_send_buf[8] */
int usbd_send(uint8_t *usbd_send_buf, char *TAG)
{
	ledb_on();
	//usbd_send_log(usbd_send_buf, TAG);

	if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbd_send_buf, sizeof(next_report)) == USBD_BUSY)	
	{
		return USBD_BUSY;
	}
	ledb_off();
	return USBD_OK;
}


void HIDInit(void)
{
  ResetReport();
	usbd_send((uint8_t *)&next_report, TAG_SEND_INIT_BUF);
	
}

void Report_Task(void)
{
	if(EasyCon_need_send_report())
	{
		usbd_send((uint8_t *)&next_report, TAG_SEND_BUF);
		EasyCon_report_send_callback();
	}
}

void HIDTask(void)
{
  // We need to run our task to process and deliver data for our IN and OUT endpoints.
  Report_Task();
}
