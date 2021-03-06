#ifndef __BSP_HT1621B_H_
#define __BSP_HT1621B_H_
#include "myinclude.h"
#include "stm8s.h"

typedef struct save_statuse
{
	u8 wind_statues;///风速的状态
	u8 sun_xue_slecet;
	u8 temperature_code;///制冷温度
	u8 temperature_hode;//制热温度
	u8 temperature_wind;//风速温度

	u8 can_id;///can总线的id号
	u8 flash_num;///记录风速自动模式闪烁位置
}Save_Statuse_T;

#define Wendu_MaxSet 32
#define Wendu_MixSet 17

#define Num_Flash_BASE 50///数字闪烁间隔时间

extern Save_Statuse_T Save_Statues;//



extern u8 show_flash_flag;//风速动态显示标志
extern u8 lcd_close_flag;//显示屏关闭标志
extern u8 Temperature_Count;//温度值
extern u8 RoomTemperature;


extern u32 Bei_Guang_Time;//背光时间
extern u32 Flash_Delay_Time;
extern u8 wind_sun_slecet;
extern u16 Show_Temp_Flash_Time;
extern u8 Show_Temp_Flash_Flag;

extern u8 Num_Flash_On_Flag;
extern u8 Num_Flash_Time;
extern u8 Clean_Show_Flag;
extern u8 Num_Flash_Data;

extern u8 peizhi_ht1621_init_on_flag;
extern u8 huifu_default_set_flag;
extern u16 AllScreen_Show_DelayTime;///所有屏幕全显时间
extern u8 AllScreen_Show_Flag;//全显标志

extern u8 flash_index;////进行闪烁的位�

extern u8 JiaoZheng_On_Flag; ///矫正开启标志




#define BEI_GUANG_TIME_BASE 1500///背光显示时间800*15.625ms=12.5s实际15s
#define FLASH_DELAY_TIME_BASE 30
#define SHOW_TEMP_FLASH_TIME_BASE 50

#define AllScreen_Show_DelayTime_BASE 500/////15.625*500ms = 7.8s


#define BLIGHT_PORT GPIOC
#define BLIGHT_PINS GPIO_PIN_3
#define BLIGHT_PIN_Hight() GPIO_WriteHigh(BLIGHT_PORT, BLIGHT_PINS)
#define BLIGHT_PIN_Low()   GPIO_WriteLow(BLIGHT_PORT,BLIGHT_PINS)

#define HT1621_CS_PORT     GPIOE
#define HT1621_CS_PINS     GPIO_PIN_5
#define HT1621_CS_Hight()  GPIO_WriteHigh(HT1621_CS_PORT, HT1621_CS_PINS)
#define HT1621_CS_Low()    GPIO_WriteLow(HT1621_CS_PORT,HT1621_CS_PINS)

#define HT1621_RD_PORT    GPIOC
#define HT1621_RD_PINS    GPIO_PIN_5
#define HT1621_RD_Hight() GPIO_WriteHigh(HT1621_RD_PORT, HT1621_RD_PINS)
#define HT1621_RD_Low()   GPIO_WriteLow(HT1621_RD_PORT,HT1621_RD_PINS)

#define HT1621_WR_PORT     GPIOC
#define HT1621_WR_PINS 	   GPIO_PIN_6
#define HT1621_WR_Hight()  GPIO_WriteHigh(HT1621_WR_PORT, HT1621_WR_PINS)
#define HT1621_WR_Low()    GPIO_WriteLow(HT1621_WR_PORT,HT1621_WR_PINS)

#define HT1621_DT_PORT      GPIOC
#define HT1621_DT_PINS      GPIO_PIN_7
#define HT1621_DT_Hight()   GPIO_WriteHigh(HT1621_DT_PORT, HT1621_DT_PINS)
#define HT1621_DT_Low()     GPIO_WriteLow(HT1621_DT_PORT,HT1621_DT_PINS)



#define   BIAS        0x50 //0b1000 0101 0010 1/2duty 4com
#define   SYSDIS      0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define   SYSEN       0X02 //0b1000 0000 0010 打开系统振荡器
#define   LCDOFF      0X04 //0b1000 0000 0100 关LCD偏压
#define   LCDON       0X06 //0b1000 0000 0110 打开LCD偏压
#define   XTAL        0x28 //0b1000 0010 1000 外部接时钟
#define   RC256       0X30 //0b1000 0011 0000 内部时钟
#define   TONEON      0X12 //0b1000 0001 0010 打开声音输出
#define   TONEOFF     0X10 //0b1000 0001 0000 关闭声音输出
#define   WDTDIS1     0X0A //0b1000 0000 1010 禁止看门狗

extern void ht1621_init(void);
void test_lcd(void);
void lcd_on(void);
void lcd_off(u8 num);
void lcd_power_off(void);
void lcd_power_on(void);//背光开

void show_flash_num(u8 num);
void my_show_char(u8 index, u8 dat);
void Delay(u16 count);
void show_scan(void);
void show_temp_flash(void);///动态显示
void display_clear(void);
void show_temp_flash2();///动态显示定时器延时
void show_temp(u8 temp);///显示温度
void display_clear_char(u8 index);


void num_flash(u8 data);////用来让两个断码管闪烁显示的函数num是控制显示还是清除
void turn_on_num_flash(void);////开启flash显示
void turn_off_num_flash(void);
void peizhi_ht1621_init(u8 num);
void load_struct(u32 addr, u8 len);///把从flash中读取的数据装载到结构体里面
void save_struct2();
void show_all_screen(void);///屏幕所有东西都显示
void display_show_char(u8 index,u8 dat);


#endif
