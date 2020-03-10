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

#define MKey_PullDown_Time_Base 300///�����8s
extern u32 MKey_PullDown_Time;
extern u8 MKey_PullDown_Flag;////m����������־��0û�а��£�1λ��������

#define UpKey_PullDown_Time_Base 300///
extern u32 UpKey_PullDown_Time;///�ϰ�������ʱ��
extern u8  UpKey_PullDown_Flag;///�ϰ����������±�־0û�а��£�1����

#define DownKey_PullDown_Time_Base 300///
extern u32 DownKey_PullDown_Time;///�°�������ʱ��
extern u8 DownKey_PullDown_Flag;////�°��������±�־��0û�а��£�1����

#define WindKey_PullDown_TimeBase 300/////
extern u32 WindKey_PullDown_Time;/////���ٳ���������
extern u8 WindKey_PullDown_Flag;///���ٳ�������־ 0û�а���1����
extern u8 WenduJiaoZhengJiLu;


extern u8 power_key_count;///��Դ��ť
extern u8 wind_key_count;///���ٰ�ť���´���
extern u8 sun_key_count;//���հ�ť���´���
extern u8 add_key_count;//�Ӱ������´���
extern u8 sub_key_count;//���������´���
extern u8 key1_delay_time;///����1������ʱʱ��
extern u8 key1_pulldown_flag;///���±�־


void scan_key(void);
void key_init(void);

#endif
