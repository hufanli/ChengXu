#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

/////////////////////
/////蜂鸣器引脚定义//////
#define BEEP_PORT   GPIOC
#define BEEP_PIN    GPIO_PIN_1
#define BEEP_ON()     GPIO_WriteHigh(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF()    GPIO_WriteLow(BEEP_PORT,BEEP_PIN)


#define BEEP_DELAY_TIME_BASE  1//
extern unsigned short Beep_Delay_Time;
extern unsigned char Use_Beep_Flag;

extern unsigned char Sys_Time;

void sys_clkconfig(void);
void systime_ctrl(void);
void judge_num_flash(void);///判断是否数字闪烁

void long_key_operate(void);
void init_show(void);///上电读取flash默认的数值进行显示

void time1_config(void);
void time4_config(void);
void time2_config(void);

void Beep_Open(void);
void Beep_Scan(void);////浏览beep是开着还是关着


///////
#endif

