#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H
#include "myinclude.h"
void adc_config(void);
u16 ADCGet(void);
float get_voltage(void);//////获取电压值
float get_res(void);//////得到电阻值
u8 get_temp(void);///获取室内温度
void save_jiaozheng_temp(void);

extern s8 JiaoZheng_Temp;///矫正温度存储位置 


#define Voltage_Vref  5.2
#define Get_Temp_Time_Base 2000 ///
extern u16 get_temp_time;///每多长时间获取一次温度

#define Temp_JiaoZheng 20 ///矫正温度存储位置

typedef struct line_tab
{
	float a;
	float b;
}line_tab_T;

#endif
