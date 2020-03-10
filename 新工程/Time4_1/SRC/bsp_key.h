#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "myinclude.h"
#include "stm8s.h"


#define POWER_KEY      GPIOB
#define POWER_KEY_PINS     GPIO_PIN_1
#define POWER_KEY_Hight()   GPIO_WriteHigh(POWER_KEY, POWER_KEY_PINS)
#define POWER_KEY_Low()     GPIO_WriteLow(POWER_KEY,POWER_KEY_PINS)


#define WIND_KEY      GPIOB
#define WIND_KEY_PINS     GPIO_PIN_2

#define SUN_KEY       GPIOB
#define SUN_KEY_PINS  GPIO_PIN_3

#define ADD_KEY       GPIOB
#define ADD_KEY_PINS  GPIO_PIN_4

#define SUB_KEY       GPIOB
#define SUB_KEY_PINS  GPIO_PIN_5

#define MKey_PullDown_Time_Base 300///大概是8s
extern u32 MKey_PullDown_Time;
extern u8 MKey_PullDown_Flag;////m按键长按标志，0没有按下，1位长按按下

#define UpKey_PullDown_Time_Base 300///
extern u32 UpKey_PullDown_Time;///上按键长按时间
extern u8  UpKey_PullDown_Flag;///上按键长按按下标志0没有按下，1按下

#define DownKey_PullDown_Time_Base 300///
extern u32 DownKey_PullDown_Time;///下按键长按时间
extern u8 DownKey_PullDown_Flag;////下按键长按下标志，0没有按下，1按下

#define WindKey_PullDown_TimeBase 300/////
extern u32 WindKey_PullDown_Time;/////风速长按键按下
extern u8 WindKey_PullDown_Flag;///风速长按键标志 0没有按下1按下
extern u8 WenduJiaoZhengJiLu;


extern u8 power_key_count;///电源按钮
extern u8 wind_key_count;///风速按钮按下次数
extern u8 sun_key_count;//光照按钮按下次数
extern u8 add_key_count;//加按键按下次数
extern u8 sub_key_count;//减按键按下次数
extern u8 key1_delay_time;///按键1消抖延时时间
extern u8 key1_pulldown_flag;///按下标志


void scan_key(void);
void key_init(void);

#endif
