#ifndef __BSP_HT1621B_H_
#define __BSP_HT1621B_H_
#include "myinclude.h"
#include "stm8s.h"

typedef struct save_statuse
{
	u8 wind_statues;///·çËÙµÄ×´Ì¬
	u8 sun_xue_slecet;
	u8 temperature_code;///ÖÆÀäÎÂ¶È
	u8 temperature_hode;//ÖÆÈÈÎÂ¶È
	u8 temperature_wind;//·çËÙÎÂ¶È

	u8 can_id;///can×ÜÏßµÄidºÅ
	u8 flash_num;///¼ÇÂ¼·çËÙ×Ô¶¯Ä£Ê½ÉÁË¸Î»ÖÃ
}Save_Statuse_T;

#define Wendu_MaxSet 32
#define Wendu_MixSet 17

#define Num_Flash_BASE 50///Êı×ÖÉÁË¸¼ä¸ôÊ±¼ä

extern Save_Statuse_T Save_Statues;//



extern u8 show_flash_flag;//·çËÙ¶¯Ì¬ÏÔÊ¾±êÖ¾
extern u8 lcd_close_flag;//ÏÔÊ¾ÆÁ¹Ø±Õ±êÖ¾
extern u8 Temperature_Count;//ÎÂ¶ÈÖµ
extern u8 RoomTemperature;


extern u32 Bei_Guang_Time;//±³¹âÊ±¼ä
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
extern u16 AllScreen_Show_DelayTime;///ËùÓĞÆÁÄ»È«ÏÔÊ±¼ä
extern u8 AllScreen_Show_Flag;//È«ÏÔ±êÖ¾

extern u8 flash_index;////½øĞĞÉÁË¸µÄÎ»Ö

extern u8 JiaoZheng_On_Flag; ///½ÃÕı¿ªÆô±êÖ¾




#define BEI_GUANG_TIME_BASE 1500///±³¹âÏÔÊ¾Ê±¼ä800*15.625ms=12.5sÊµ¼Ê15s
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
#define   SYSDIS      0X00 //0b1000 0000 0000 ¹ØÕñÏµÍ³µ´Æ÷ºÍLCDÆ«Ñ¹·¢ÉúÆ÷
#define   SYSEN       0X02 //0b1000 0000 0010 ´ò¿ªÏµÍ³Õñµ´Æ÷
#define   LCDOFF      0X04 //0b1000 0000 0100 ¹ØLCDÆ«Ñ¹
#define   LCDON       0X06 //0b1000 0000 0110 ´ò¿ªLCDÆ«Ñ¹
#define   XTAL        0x28 //0b1000 0010 1000 Íâ²¿½ÓÊ±ÖÓ
#define   RC256       0X30 //0b1000 0011 0000 ÄÚ²¿Ê±ÖÓ
#define   TONEON      0X12 //0b1000 0001 0010 ´ò¿ªÉùÒôÊä³ö
#define   TONEOFF     0X10 //0b1000 0001 0000 ¹Ø±ÕÉùÒôÊä³ö
#define   WDTDIS1     0X0A //0b1000 0000 1010 ½ûÖ¹¿´ÃÅ¹·

extern void ht1621_init(void);
void test_lcd(void);
void lcd_on(void);
void lcd_off(u8 num);
void lcd_power_off(void);
void lcd_power_on(void);//±³¹â¿ª

void show_flash_num(u8 num);
void my_show_char(u8 index, u8 dat);
void Delay(u16 count);
void show_scan(void);
void show_temp_flash(void);///¶¯Ì¬ÏÔÊ¾
void display_clear(void);
void show_temp_flash2();///¶¯Ì¬ÏÔÊ¾¶¨Ê±Æ÷ÑÓÊ±
void show_temp(u8 temp);///ÏÔÊ¾ÎÂ¶È
void display_clear_char(u8 index);


void num_flash(u8 data);////ÓÃÀ´ÈÃÁ½¸ö¶ÏÂë¹ÜÉÁË¸ÏÔÊ¾µÄº¯ÊınumÊÇ¿ØÖÆÏÔÊ¾»¹ÊÇÇå³ı
void turn_on_num_flash(void);////¿ªÆôflashÏÔÊ¾
void turn_off_num_flash(void);
void peizhi_ht1621_init(u8 num);
void load_struct(u32 addr, u8 len);///°Ñ´ÓflashÖĞ¶ÁÈ¡µÄÊı¾İ×°ÔØµ½½á¹¹ÌåÀïÃæ
void save_struct2();
void show_all_screen(void);///ÆÁÄ»ËùÓĞ¶«Î÷¶¼ÏÔÊ¾
void display_show_char(u8 index,u8 dat);


#endif
