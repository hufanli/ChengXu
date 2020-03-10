#include "temp_sensor.h"
#include "myinclude.h"
#include "stm8s_adc1.h"


#define MF52E////3960 10K

u16 get_temp_time = 0;///每多长时间获取一次温度

s8 JiaoZheng_Temp = 0;///矫正温度存储位置 
float LastRes = 0.0;///存储上次获取的电阻值

u8 LastTemper = 0;///记录上次测试温度值


////每5度做一个线性函数,这样更精准y = kx+b;y 为温度，x为电阻做一个线性表格
 line_tab_T myling_tab[19] = {{-0.077882 ,-20.685288},  {-0.108249, -15.103855}, 
 							  {-0.149209, -9.4673431},  {-0.203252, -3.841466},
 							  {-0.274273, 1.80470671},  {-0.366300, 7.443206},
 							  {-0.483559, 13.02707952}, {-0.635324, 18.68487884},
 							  {-0.826446, 24.30577856}, {-1.070435, 30.005359715},
 							  {-1.375138, 35.70682800}, {-1.769912, 41.651333496},
 							  {-2.239140, 47.3914},     {-2.791737, 52.917369033},
 							  {-3.489184, 58.6392216},  {-4.336513, 64.384212599},
 							  {-5.353319, 70.101712791},{-6.578947, 75.848682763},};

typedef struct Temp_Tab{
    float MaxRes;
    float MinRes;
    u8 temper;
}Temp_Tab_T;

Temp_Tab_T Temp_3950Tab[] = {
    {34.46, 31.11, 0},  {32.79, 29.53, 1},  {31.11, 28.05, 2},
    {29.53, 26.65, 3},  {28.05, 25.34, 4},  {26.65, 24.11, 5},
    {25.34, 22.94, 6},  {24.11, 21.84, 7},  {22.94, 20.81, 8},
    {21.84, 19.83, 9},  {20.81, 18.90, 10}, {19.83, 18.02, 11},
    {18.90, 17.19, 12}, {18.02, 16.41, 13}, {17.19, 15.66, 14},
    {16.41, 14.96, 15}, {15.66, 14.29, 16}, {14.96, 13.65, 17},
    {14.29, 13.05, 18}, {13.65, 12.47, 19}, {13.05, 11.93, 20},
    {12.47, 11.41, 21}, {11.93, 10.92, 22}, {11.41, 10.45, 23},
    {10.92, 10.00, 24}, {10.45, 9.575, 25}, {10.00, 9.170, 26},
    {9.575, 8.784, 27}, {9.170, 8.416, 28}, {8.784, 8.065, 29},
    {8.416, 7.731, 30}, {8.065, 7.412, 31}, {7.731, 7.107, 32},
    {7.412, 6.817, 33}, {7.107, 6.539, 34}, {6.817, 6.274, 35},
    {6.539, 6.021, 36}, {6.274, 5.780, 37}, {6.021, 5.549, 38},
    {5.780, 5.328, 39}, {5.549, 5.118, 40}, {5.328, 4.916, 41},
    {5.118, 4.723, 42}, {4.916, 4.539, 43}, {4.723, 4.362, 44},
    {4.539, 4.194, 45}, {4.362, 4.032, 46}, {4.194, 3.878, 47}
    
};

void adc_config(void)
{
	GPIO_Init(GPIOB, GPIO_PIN_0,GPIO_MODE_IN_FL_NO_IT);
	ADC1_DeInit();


    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D6, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL8, DISABLE);
		
	ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
	ADC1_Cmd(ENABLE);                
	ADC1_StartConversion(); 
}


u16 ADCGet(void)
{	
    FlagStatus ADCRdy = RESET;
	u8 n=0;
  u16 mid = 0;
  u16 max = 0;
  u16 min = 4096;
  u32 temp=0;
  for(n=0;n<100;n++)
  {
    do
    {
        ADCRdy = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
    }
    while(ADCRdy == RESET); // SET or RESET
    ADC1_ClearFlag(ADC1_FLAG_EOC);
    mid = ADC1_GetConversionValue();
    temp += mid;
    if(mid>max) max =mid;
    if(mid<min) min =mid;
  }
  temp =temp - mid - min;
  temp /=98;
  return (u16)temp;

}


float  get_voltage(void)//////获取电压值
{
	float  temp1, temp2;
	u16 ad_count = 0;
	ad_count = ADCGet();
	temp1 = (float)(ad_count)/1024;
	temp2 = (float)(temp1)*(Voltage_Vref);

	return temp2;
}


#if 0
float get_res(void)//////得到电阻值
{
	float temp = 0.0, temp2;
	float res;
	float LastResTemp = 0.0;
	
	temp2 = get_voltage();

	//temp = (float)Voltage_Vref*10;///这里直接除掉1000
	temp = (float)temp2*10;
	res = (float)temp/(Voltage_Vref-temp2);

    LastResTemp = res;
    res = (LastRes+res)/2;
  
    LastRes = LastResTemp;
	return res;
}
#endif

float get_res(void)//////得到电阻值
{
	float temp = 0.0, temp2;
	float res;
	float LastResTemp = 0.0;
	
	temp2 = get_voltage();

	//temp = (float)Voltage_Vref*10;///这里直接除掉1000
	temp = (float)temp2*10;
	res = (float)temp/(Voltage_Vref-temp2);

    //LastResTemp = res;
    //res = (LastRes+res)/2;
  
    //LastRes = LastResTemp;
	return res;
}



#ifndef MF52E
u8 get_temp(void)///获取室内温度
{
	float res, temp;
	u8 temp2;
	
	res = get_res();///获取电阻值
	//res = 12.68;
	//res = (float)res/1000;
	if((res < (float)248.0)&&(res> (float)183.8))///-35   -40
	{
		temp = myling_tab[0].a*res+myling_tab[0].b;
	}
	else if((res < (float)183.8)&&(res> (float)137.61))////-30   -35
	{
		temp = myling_tab[1].a*res+myling_tab[1].b;
	}
	else if((res < (float)137.61)&&(res> (float)104.0))///-25 -30
    {
    	temp = myling_tab[2].a*res+myling_tab[2].b;
    }
	else if((res < (float)104.0)&&(res> (float)79.5))////-20   -25
	{
		temp = myling_tab[3].a*res+myling_tab[3].b;
	}
	else if((res < (float)79.5)&&(res> (float)61.27))////-15  -20
	{
		temp = myling_tab[4].a*res+myling_tab[4].b;
	}
	else if((res < (float)61.27)&&(res> (float)47.62))////-10  -15
	{
		temp = myling_tab[5].a*res+myling_tab[5].b;
	}
	else if((res < (float)47.62)&&(res> (float)37.28))////-5  -10
	{
		temp = myling_tab[6].a*res+myling_tab[6].b;
	}
	else if((res < (float)37.28)&&(res> (float)29.41))////0  -5
	{
		temp = myling_tab[7].a*res+myling_tab[7].b;
	}
	else if((res < (float)29.41)&&(res> (float)23.36))////5   0
	{
		temp = myling_tab[8].a*res+myling_tab[8].b;
	}
	else if((res < (float)23.36)&&(res> (float)18.689))////10    5
	{
		temp = myling_tab[9].a*res+myling_tab[9].b;
	}
	else if((res < (float)18.689)&&(res> (float)15.058))////15    10
	{
		temp = myling_tab[10].a*res+myling_tab[10].b;
	}
	else if((res < (float)15.058)&&(res> (float)12.233))////20    15
	{
		temp = myling_tab[11].a*res+myling_tab[11].b;
	}
	else if((res < (float)12.233)&&(res> (float)10))////25   20
	{
		temp = myling_tab[12].a*res+myling_tab[12].b;
	}
	else if((res < (float)10)&&(res> (float)8.209))////30   25
	{
		temp = myling_tab[13].a*res+myling_tab[13].b;
	}
	else if((res < (float)8.209)&&(res> (float)6.776))////35  30
	{
		temp = myling_tab[14].a*res+myling_tab[14].b;
	}
	else if((res < (float)6.776)&&(res> (float)5.623))////40  35
	{
		temp = myling_tab[15].a*res+myling_tab[15].b;
	}
	else if((res < (float)5.623)&&(res> (float)4.689))////45  40
	{
		temp = myling_tab[16].a*res+myling_tab[16].b;
	}
	else if((res < (float)4.689)&&(res> (float)3.929))////50  45
	{
		temp = myling_tab[17].a*res+myling_tab[17].b;
	}
	temp2 = (u8)temp;
	if((float)(temp-temp2)>0.5)
	{
		temp2 = (u8)temp+1;
	}
	else
	{
		temp2 = (u8)temp;
	}
///////////////////加上一介滞后滤波@取值0.2
    if(LastTemper == 0)
    {
        LastTemper = temp2;
    }
    temp2 = (LastTemper*0.2)+((1-0.2)*temp2);
    LastTemper = temp2;
	return (u8)temp2;
}
#else
//// 此函适用MF52E 10k B值3950 精度1%
u8 get_temp(void)
{
    float res;
	u8 temp;
    u8 i = 0;
    
    res = get_res();///获取电阻值
    
    for(i = 0; i < 50; i++)
    {
        if((res <= Temp_3950Tab[i].MaxRes)&&(res >= Temp_3950Tab[i].MinRes))
        {
            temp = Temp_3950Tab[i].temper;
            break;
        }
    }

    return temp;
    
}

#endif
void save_jiaozheng_temp(void)
{
	u8 temp[2] = {0};
	if(JiaoZheng_Temp > 0)
	{
		temp[0] = 0;
		temp[1] = (u8)abs(JiaoZheng_Temp);
	}
	else
	{
		temp[0] = 1;
		temp[1] = (u8)abs(JiaoZheng_Temp);
	}
	write_flash((u32)(FLASH_ADDR_BASE+Temp_JiaoZheng), temp, 2);
}
