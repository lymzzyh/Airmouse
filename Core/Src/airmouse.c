#include <airmouse.h>
#include <main.h>

#define Button_L 0x01
#define Button_R 0x02
#define Button_M 0x04

#define Move_X 1
#define Move_Y 2
#define Move_S 3

extern USBD_HandleTypeDef hUsbDeviceFS;

void build_report(uint8_t *buf,uint8_t button,uint8_t scroll,uint8_t x,uint8_t y)
{
    buf[0]=button;
    buf[1]=x;
    buf[2]=y;
    buf[3]=scroll;
}

void clean_report(uint8_t *buf)
{
    buf[0]=0;
    buf[1]=0;
    buf[2]=0;
    buf[3]=0;
}

void Click_L()
{
	uint8_t buf0[4] = {1,0,0,0};
	uint8_t buf1[4] = {0,0,0,0};
	USBD_HID_SendReport(&hUsbDeviceFS,buf0,4);//按下鼠标左键
	HAL_Delay(10);
	USBD_HID_SendReport(&hUsbDeviceFS,buf1,4);//松开鼠标按键
	HAL_Delay(10);
}
void Click_R()
{
	uint8_t buf0[4] = {2,0,0,0};
	uint8_t buf1[4] = {0,0,0,0};
	USBD_HID_SendReport(&hUsbDeviceFS,buf0,4);//按下鼠标右键
	HAL_Delay(10);
	USBD_HID_SendReport(&hUsbDeviceFS,buf1,4);//松开鼠标按键
	HAL_Delay(10);
}
void Click_M()
{
	uint8_t buf0[4] = {4,0,0,0};
	uint8_t buf1[4] = {0,0,0,0};
	USBD_HID_SendReport(&hUsbDeviceFS,buf0,4);//按下鼠标中键
	HAL_Delay(10);
	USBD_HID_SendReport(&hUsbDeviceFS,buf1,4);//松开鼠标按键
	HAL_Delay(10);
}
void Scroll(int8_t x)
{
	uint8_t buf[4] = {0,0,0,0};
	buf[3]=x;
	USBD_HID_SendReport(&hUsbDeviceFS,buf,4);//鼠标滚动
	HAL_Delay(10);
}
void Move(int8_t x,int8_t y)
{
	uint8_t buf[4] = {0,0,0,0};
	buf[1]=x;
	buf[2]=y;
	USBD_HID_SendReport(&hUsbDeviceFS,buf,4);//鼠标移动
	HAL_Delay(10);
}
/* USER CODE END 1 */
