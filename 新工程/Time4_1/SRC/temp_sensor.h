#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H
#include "myinclude.h"
void adc_config(void);
u16 ADCGet(void);
float get_voltage(void);//////��ȡ��ѹֵ
float get_res(void);//////�õ�����ֵ
u8 get_temp(void);///��ȡ�����¶�
void save_jiaozheng_temp(void);

extern s8 JiaoZheng_Temp;///�����¶ȴ洢λ�� 


#define Voltage_Vref  5.2
#define Get_Temp_Time_Base 2000 ///
extern u16 get_temp_time;///ÿ�೤ʱ���ȡһ���¶�

#define Temp_JiaoZheng 20 ///�����¶ȴ洢λ��

typedef struct line_tab
{
	float a;
	float b;
}line_tab_T;

#endif
