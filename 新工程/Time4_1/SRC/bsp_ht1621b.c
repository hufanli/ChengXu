#include "bsp_ht1621b.h"
#include "stm8s.h"
#include "myinclude.h"

u8 Temperature_Count = 23;///ÓÃÀ´ÏÔÊ¾ÊÒÎÂÓÃ
u8 RoomTemperature = 26;/////ÊÒÄÚÎÂ¶È±äÁ¿

u32 Bei_Guang_Time = (u32)0;
u32 Flash_Delay_Time = (u32)0;//flashÑÓÊ±
u8 wind_sun_slecet = 0;//0 Ã»ÓĞÑ¡Ôñ£¬1Ñ¡ÔñÑ©»¨£¬2Ñ¡ÔñÑô¹â
u16 Show_Temp_Flash_Time = 0;///µ±ĞŞ¸ÄÎÂ¶ÈÊ±ÉÁË¸¼ä¸ôÊ±¼ä

u8 LCD_RAM[18] = {0};
u8 show_flash_flag = 0;///flash ÏÔÊ¾±êÖ¾
u8 lcd_close_flag = 1;//Õû¸öÏÔÊ¾¹Øµô±êÖ¾
u8 Bg_Light_flag = 0;//±³¹â¿ª±Õ±êÖ¾0±Õ1¿ª
u8 Show_Temp_Flash_Flag = 0;///ÎÂ¶ÈÏÔÊ¾ÉÁË¸±êÖ¾0²»ÉÁË¸£¬1ÉÁË¸

u8 Num_Flash_On_Flag = 0;////¿ªÊıÂë¹ÜÉÁË¸±êÖ¾
u8 Num_Flash_Time = 0;//Êı×ÖÉÁË¸¼ä¸ôÊ±¼ä
u8 Clean_Show_Flag = 0;//Çå³ıÏÔÊ¾±êÖ¾
u8 Num_Flash_Data = 0;///ÓÃÀ´ÏÔÊ¾µÄÊıÖµ

u8 peizhi_ht1621_init_on_flag = 0;///ÅäÖÃ1621ÏÔÊ¾ÆÁ¿ªÆô±êÖ¾
u8 huifu_default_set_flag = 0;////»Ö¸´Ä¬ÈÏµÄÉèÖÃ

u8 JiaoZheng_On_Flag = 0; ///½ÃÕı¿ªÆô±êÖ¾

u8 AllScreen_Show_Flag = 0;//È«ÏÔ±êÖ¾
u16 AllScreen_Show_DelayTime = 0;///ËùÓĞÆÁÄ»È«ÏÔÊ±¼ä

u8 flash_index = 27;////½øĞĞÉÁË¸µÄÎ»ÖÃ

Save_Statuse_T Save_Statues;//


//0    1    2    3     4     5   6     7    8     9    Çå³şaf
const u8 NUM_LAB[35] = {0xaf, 0xa0, 0xcb, 0xe9, 0xe4, 0x6d, 0x6f, 0xa8, 0xef, 0xed, 0x00,//10
			  		//  -     --    ---   ----  ----- ------       auto  C		 
                       0x0a, 0x09, 0x18, 0x28, 0x48, 0x88, 0x0c, 0x08,//18
                      0x0b, 0x1b, 0x3b, 0x7b, 0xfb, //23  ·Ö±ğÊÇ2¸öºáÒ»´Î
                     //µçÔ´ Ñ©»¨ Ñô¹â
                       0x20, 0x60, 0xa0,///26
                       0x0e,0x0f, 0x1f, 0x3f, 0x7f, 0xff,////32
						///-
						0x40, 0x4f};// 0x4fÏÔÊ¾E

/*
//0    1    2    3     4     5   6     7    8     9    Çå³ş
const u8 NUM_LAB[22] = {0x80, 0x06, 0xcb, 0x4f, 0x66, 0x6d, 0xed, 0x07, 0xef, 0x67, 0x00,
			  		//  -     --    ---   ----  ----- ------       auto  C		 
                       0x02, 0x01, 0x10, 0x20, 0x40, 0x80, 0x04, 0x08,
                     //µçÔ´ Ñ©»¨ Ñô¹â
                       0x20, 0x60, 0xa0}; */

//u8 FLAG[3] = {0x00,0x00,0x00};
//u8 loadflag[4] = {0};

void Delay(u16 count)
{
	
	while(count)
	{
		count--;
	}
}


void flash_delay(void)
{	
	u8 i = 0;
	for(i = 0; i < 10; i++)
		Delay(4000);
}

void load_struct(u32 addr, u8 len)///°Ñ´ÓflashÖĞ¶ÁÈ¡µÄÊı¾İ×°ÔØµ½½á¹¹ÌåÀïÃæ
{
  u8 temp[Default_Data_Len] = {0} ;
	read_flash(addr, temp, (u8)len);
	Save_Statues.wind_statues = temp[0];
	Save_Statues.sun_xue_slecet = temp[1];
	Save_Statues.temperature_code = temp[2];
	Save_Statues.temperature_hode = temp[3];
	Save_Statues.temperature_wind = temp[4];
	Save_Statues.can_id = temp[5];
	Save_Statues.flash_num = temp[6];
}

void save_struct()/////°ÑstructµÄÊı¾İ´æµ½flash
{
	u8 temp[Default_Data_Len] = {0};///6¸ö×´Ì¬
	Save_Statues.wind_statues = wind_key_count;
	Save_Statues.sun_xue_slecet = sun_key_count;//±£´æÊÇ¼ÓÈÈ»¹ÊÇÖÆÀä
	temp[0] = Save_Statues.wind_statues;
	temp[1] = Save_Statues.sun_xue_slecet;
	temp[2] = Save_Statues.temperature_code;
	temp[3] = Save_Statues.temperature_hode;
	temp[4] = Save_Statues.temperature_wind;
	temp[5] = Save_Statues.can_id;
	temp[6] = Save_Statues.flash_num;
	write_flash((u32)(FLASH_ADDR_BASE+Set_Data_Offset), temp, (u8)Default_Data_Len);
	//Save_Statues->temperature_code
}

void save_struct2()
{
	u8 temp[Default_Data_Len] = {0};///6¸ö×´Ì¬
	//Save_Statues.wind_statues = wind_key_count;
	//Save_Statues.sun_xue_slecet = sun_key_count;//±£´æÊÇ¼ÓÈÈ»¹ÊÇÖÆÀä
	temp[0] = Save_Statues.wind_statues;
	temp[1] = Save_Statues.sun_xue_slecet;
	temp[2] = Save_Statues.temperature_code;
	temp[3] = Save_Statues.temperature_hode;
	temp[4] = Save_Statues.temperature_wind;
	temp[5] = Save_Statues.can_id;
	temp[6] = Save_Statues.flash_num;
	write_flash((u32)(FLASH_ADDR_BASE+Set_Data_Offset), temp, (u8)Default_Data_Len);
}

//-----------------------------------------------------------------------------------------
//º¯ÊıÃû³Æ£ºHt1621Wr_Data()
//¹¦ ÄÜ£ºĞ´Êı¾İº¯Êı,cntÎª´«ËÍÊı¾İÎ»Êı,Êı¾İ´«ËÍÎªµÍÎ»ÔÚÇ°
//----------------------------------------------------------------------------------------
void Ht1621Wr_Data(u8 Data,u8 cnt)
{
#if 1
  u8 i = 0;
  
  for (i=0;i<cnt;i++)
  {
    HT1621_WR_Low();//HT1621_WR=0;
    //      _delay_us(20);
    if((Data & 0x80)==0x80)
      HT1621_DT_Hight();//HT1621_IO=1;
    else
      HT1621_DT_Low();//HT1621_IO=0;
    
    HT1621_WR_Hight();//HT1621_WR=1;
    //      _delay_us(20);
    Data<<=1;
  }
  //   _delay_us(50);
  #endif
}

//-----------------------------------------------------------------------------------------
//º¯ÊıÃû³Æ£ºvoid Ht1621WrOneData(uint8_t Addr,uint8_t Data)
//¹¦ÄÜÃèÊö: HT1621ÔÚÖ¸¶¨µØÖ·Ğ´ÈëÊı¾İº¯Êı
//²ÎÊıËµÃ÷£ºAddrÎªĞ´Èë³õÊ¼µØÖ·£¬DataÎªĞ´ÈëÊı¾İ
//Ëµ Ã÷£ºÒòÎªHT1621µÄÊı¾İÎ»4Î»£¬ËùÒÔÊµ¼ÊĞ´ÈëÊı¾İÎª²ÎÊıµÄºó4Î»
//-----------------------------------------------------------------------------------------
void Ht1621WrOneData(u8 Addr,u8 Data)
{    
  HT1621_CS_Low();     //HT1621_CS=0;
  Ht1621Wr_Data(0xa0,3);    //Ğ´ÈëÊı¾İ±êÖ¾101
  Ht1621Wr_Data(Addr<<2,6); //Ğ´ÈëµØÖ·Êı¾İ
  Ht1621Wr_Data(Data<<4,4); //Ğ´ÈëÊı¾İ
  HT1621_CS_Hight();   //HT1621_CS=1;
}

//-----------------------------------------------------------------------------------------
//º¯ÊıÃû³Æ£ºvoid Ht1621WrCmd(uint8_t Cmd)
//¹¦ÄÜÃèÊö: HT1621ÃüÁîĞ´Èëº¯Êı
//²ÎÊıËµÃ÷£ºCmdÎªĞ´ÈëÃüÁîÊı¾İ
//Ëµ Ã÷£ºĞ´ÈëÃüÁî±êÊ¶Î»100
//-----------------------------------------------------------------------------------------
void Ht1621WrCmd(u8 Cmd)
{
  HT1621_CS_Low();     //HT1621_CS=0;
  //    _delay_us(50);
  Ht1621Wr_Data(0x80,4);    //Ğ´ÈëÃüÁî±êÖ¾100
  Ht1621Wr_Data(Cmd,8);     //Ğ´ÈëÃüÁîÊı¾İ
  HT1621_CS_Hight();   //HT1621_CS=1;
}

void lcd_power_on(void)//±³¹â¿ª
{
    #if 1
	//GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_LOW_FAST);
	if(Bg_Light_flag == 0)
	{
		BLIGHT_PIN_Low();
		Bg_Light_flag = 1;
	}
    #endif
}

void lcd_power_off(void)//±³¹â¹Ø
{
    #if 1
	//GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_LOW_FAST);
	BLIGHT_PIN_Hight();
	if(Bg_Light_flag == 1)
	{
		BLIGHT_PIN_Hight();
		Bg_Light_flag = 0;
	}
    #endif
}

void Ht1621_Write_X_number(u8 x, u8 number)
{
  u8 i = 0;
  i = 2*x;
  
  LCD_RAM[i] = (NUM_LAB[number]>>4);
  Ht1621WrOneData(i,LCD_RAM[i]);
  i++;
  LCD_RAM[i] = (NUM_LAB[number]&0x0f);
  Ht1621WrOneData(i,LCD_RAM[i]);
  
}




/**
º¯ÊıËµÃ÷:ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û
²Î    Êı:
index:ÏÔÊ¾Î»ÖÃ1-8
dat:ÏÔÊ¾×Ö·û
·µ »Ø Öµ:-
*/
void display_show_char(u8 index,u8 dat)
{
	
  		Ht1621_Write_X_number(index,dat);

}

void my_show_char(u8 index, u8 dat)
{
	u8 i = 0;
  i = 2*index;
  
  LCD_RAM[i] = (dat>>4);
  Ht1621WrOneData(i,LCD_RAM[i]);
  i++;
  LCD_RAM[i] = (dat&0x0f);
  Ht1621WrOneData(i,LCD_RAM[i]);
}

/**
º¯ÊıËµÃ÷:ÔÚÖ¸¶¨Î»ÖÃ²Á³ıÒ»¸ö×Ö·û
²Î    Êı:
index:²Á³ıÎ»ÖÃ1-8
·µ »Ø Öµ:-
*/
void display_clear_char(u8 index)
{
  Ht1621_Write_X_number(index,0x0A);
  
 
}

/**
º¯ÊıËµÃ÷:ÇåÆÁ
²Î    Êı:-	
·µ »Ø Öµ:-
*/
void display_clear(void)
{
  u8 i=0;
  
  for(i=0;i<4;i++)
    display_clear_char(i);
}

void show_temp(u8 temp)///ÏÔÊ¾ÎÂ¶È
{
	u8 gewei = 0, shiwei = 0;
    //if(temp <=40)
    {
    	gewei = temp%10;
    	shiwei = temp/10;
    	display_show_char(1,gewei);
    	display_show_char(2,shiwei);
    }
    #if 0
    else
    {
        display_show_char(2,34);///ÏÔÊ¾E
    	display_show_char(1,0);///0
    }
	#endif
}

void show_temp_flash(void)///¶¯Ì¬ÏÔÊ¾
{
	display_show_char(0, 17);//Çå³ı
	
	display_show_char(0, 27);////-
	flash_delay();
	display_show_char(0, 28);///--
	flash_delay();
	display_show_char(0, 29);
	flash_delay();
	display_show_char(0, 30);
	flash_delay();
	display_show_char(0, 31);
	flash_delay();
	display_show_char(0, 32);
	flash_delay();	
	display_show_char(0, 17);//Çå³ı
}


void show_temp_flash2()///¶¯Ì¬ÏÔÊ¾
{
	
	//display_show_char(0, 17);//Çå³ı
	if(lcd_close_flag == 0)
	{
	if(Flash_Delay_Time == 0)
	{
		if(flash_index == 33)
		{
			flash_index = 27;
		}
		Save_Statues.flash_num = flash_index;
		display_show_char(0, flash_index);////-
		flash_index++;
		
		Flash_Delay_Time = (u32)FLASH_DELAY_TIME_BASE;
	}
	}
	
	
}




void show_flash_num(u8 num)
{
	display_show_char(0, 18);///ÏÈÇåµô×Ô¶¯
	if(num == 0)
		display_show_char(0, 19);////2
	if(num == 1)
		display_show_char(0, 21);////4
	if(num == 2)
		display_show_char(0, 23);////6
}

void init_show(void)///ÉÏµç¶ÁÈ¡flashÄ¬ÈÏµÄÊıÖµ½øĞĞÏÔÊ¾
{
	load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);

	init_all_data(1);
	//Ä£Ê½ÅĞ¶Ï
	if(Save_Statues.sun_xue_slecet == 2)////Ñ©»¨ÖÆÀä
	{
		display_show_char(3, 25);///
		wind_sun_slecet = 1;
		sun_key_count = 1;
	}
	else if(Save_Statues.sun_xue_slecet == 4)///Ñô¹âÖÆÈÈ
	{
		display_show_char(3, 26);
		wind_sun_slecet = 2;
		sun_key_count = 3;
	}
	else if((Save_Statues.sun_xue_slecet == 6)||(Save_Statues.sun_xue_slecet ==0))///ÎŞÏÔÊ¾
	{
		display_show_char(3, 24);
		wind_sun_slecet = 0;
		
		sun_key_count =  5;
	}

	////ÏÔÊ¾ÊÒÄÚÎÂ¶È
	show_temp(RoomTemperature);
	//·çËÙÅĞ¶Ï
	if((Save_Statues.wind_statues == 8)||(Save_Statues.wind_statues == 0))//¶¯Ì¬ÏÔÊ¾·çËÙ
	{
		show_temp_flash2();///¶¯Ì¬ÏÔÊ¾--
		show_flash_flag = 1;
		wind_key_count = 0;
	}
	else if(Save_Statues.wind_statues == 2)////2¸ñ·çËÙ
	{
		show_flash_num(0);////2¸ñ
		wind_key_count = 1;
	}
	else if(Save_Statues.wind_statues == 4)////4¸ñ·çËÙ
	{
		show_flash_num(1);////4¸ñ
		wind_key_count = 3;
	}
	else if(Save_Statues.wind_statues == 6)////2¸ñ·çËÙ
	{
		show_flash_num(2);////6¸ñ
		wind_key_count = 5;
		
	}


	/////ÒªĞŞ¸Ä°´¼üÏÖÔÚ¶ÔÓ¦°´µÄÎ»ÖÃ
	
	
	
	display_show_char(0,18);///ÏÔÊ¾ÎÂ¶È
	//show_temp_flash();///¶¯Ì¬ÏÔÊ¾--
}
void ht1621_init(void)
{
	u8 i = 0;
	GPIO_Init(HT1621_CS_PORT, HT1621_CS_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);//cs
	GPIO_Init(HT1621_DT_PORT, HT1621_DT_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(HT1621_WR_PORT, HT1621_WR_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(HT1621_RD_PORT, HT1621_RD_PINS, GPIO_MODE_IN_PU_NO_IT);

	lcd_power_on();

	HT1621_CS_Hight();
	HT1621_RD_Hight();
	HT1621_WR_Hight();
	HT1621_DT_Hight();

 	Ht1621WrCmd(BIAS);
  	Ht1621WrCmd(RC256); //Ê¹ÓÃÄÚ²¿Õñµ´Æ÷
  	Ht1621WrCmd(SYSDIS);  //¹ØÕñÏµÍ³µ´Æ÷ºÍLCDÆ«Ñ¹·¢ÉúÆ÷
  	Ht1621WrCmd(WDTDIS1); //½ûÖ¹¿´ÃÅ¹·
  	Ht1621WrCmd(SYSEN); //´ò¿ªÏµÍ³Õñµ´Æ÷
  	Ht1621WrCmd(LCDON); //´ò¿ªLCDÆ«Ñ¹

    
	HT1621_CS_Low();
	Ht1621Wr_Data(0xa0,3); //Ğ´ÈëÊı¾İ±êÖ¾101
  	Ht1621Wr_Data(0x00<<2,6); //Ğ´ÈëµØÖ·Êı¾İ
	for (i=0;i<18;i++)
  	{
    	Ht1621Wr_Data(0x00,8); //Ğ´ÈëÊı¾İ
  	}
  	HT1621_CS_Hight();//HT1621_CS=1;  
	display_clear();///ÇåÆÁ
	init_show();////³õÊ¼»¯ÏÔÊ¾ÖØĞÂ°Ñ½á¹¹ÌåÖĞµÄÊı¾İ½øĞĞÏÔÊ¾
	//show_flash_flag = 1;
	  
}

void peizhi_ht1621_init(u8 num)///³¤°´ÅäÖÃÊ±Òª³õÊ¼»¯ÏÔÊ¾ÆÁµÄ½Ó¿Ú
{
	u8 i = 0;
	GPIO_Init(HT1621_CS_PORT, HT1621_CS_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);//cs
	GPIO_Init(HT1621_DT_PORT, HT1621_DT_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(HT1621_WR_PORT, HT1621_WR_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(HT1621_RD_PORT, HT1621_RD_PINS, GPIO_MODE_IN_PU_NO_IT);

	lcd_power_on();

	HT1621_CS_Hight();
	HT1621_RD_Hight();
	HT1621_WR_Hight();
	HT1621_DT_Hight();

 	Ht1621WrCmd(BIAS);
  	Ht1621WrCmd(RC256); //Ê¹ÓÃÄÚ²¿Õñµ´Æ÷
  	Ht1621WrCmd(SYSDIS);  //¹ØÕñÏµÍ³µ´Æ÷ºÍLCDÆ«Ñ¹·¢ÉúÆ÷
  	Ht1621WrCmd(WDTDIS1); //½ûÖ¹¿´ÃÅ¹·
  	Ht1621WrCmd(SYSEN); //´ò¿ªÏµÍ³Õñµ´Æ÷
  	Ht1621WrCmd(LCDON); //´ò¿ªLCDÆ«Ñ¹

	HT1621_CS_Low();
	Ht1621Wr_Data(0xa0,3); //Ğ´ÈëÊı¾İ±êÖ¾101
  	Ht1621Wr_Data(0x00<<2,6); //Ğ´ÈëµØÖ·Êı¾İ
	for (i=0;i<18;i++)
  	{
    	Ht1621Wr_Data(0x00,8); //Ğ´ÈëÊı¾İ
  	}
  	HT1621_CS_Hight();//HT1621_CS=1;  
  	if(num == 0)
  	{
  		peizhi_ht1621_init_on_flag = 1;///ËµÃ÷¿ªÆôÁË
  		lcd_close_flag = 0;////ËµÃ÷ÆÁÄ»¿ªÆô
  	}
	else if(num == 1)////»Ö¸´Ä¬ÈÏ
	{
		huifu_default_set_flag = 1;///
		lcd_close_flag = 0;////ËµÃ÷ÆÁÄ»¿ªÆô
	}
	else if(num == 2)///È«ÏÔ
	{
		AllScreen_Show_Flag = 1;
		lcd_close_flag = 0;////ËµÃ÷ÆÁÄ»¿ªÆô
	}
	else if(num == 3)
	{
		JiaoZheng_On_Flag = 1;
		lcd_close_flag = 1;////ËµÃ÷ÆÁÄ»¿ªÆô
	}
	
	lcd_close_flag = 0;////ËµÃ÷ÆÁÄ»¿ªÆô
	display_clear();///ÇåÆÁ
}


void lcd_on(void)
{
	ht1621_init();
	lcd_close_flag = 0;
}

void lcd_off(u8 num)
{
	//u8 temp[20] = {0};
	
	display_clear();///ÇåÆ
	Ht1621WrCmd(LCDOFF);
	HT1621_CS_Hight();
	HT1621_RD_Low();
	HT1621_WR_Low();
	HT1621_DT_Low();

	lcd_power_off();
	/*
	Save_Statues.wind_statues = wind_key_count;
	Save_Statues.sun_xue_slecet = sun_key_count;//±£´æÊÇ¼ÓÈÈ»¹ÊÇÖÆÀä
	temp[0] = Save_Statues.wind_statues;
	temp[1] = Save_Statues.sun_xue_slecet;
	temp[2] = Save_Statues.temperature_code;
	temp[3] = Save_Statues.temperature_hode;
	temp[4] = Save_Statues.temperature_wind;
	temp[5] = Save_Statues.can_id;
	write_flash((u32)(FLASH_ADDR_BASE+Default_Set_Offset), temp, (u8)Default_Data_Len);
	*/
	if(num == 0)///num 
		save_struct();
	//Save_Statues->temperature_code
//	Save_Statues.temperature = Temperature_Count;
//	Save_Statues.wind_statues = wind_key_count;
	
	lcd_close_flag = 1;
	Bei_Guang_Time = 0;//ÇåÁã·ÀÖ¹×ÔÆô¶¯
	Show_Temp_Flash_Flag = 0;//¹ØµôÉÁË¸±êÖ¾
	peizhi_ht1621_init_on_flag = 0;///ÅäÖÃµÄ¹Ø±ÕÒ²µ÷ÓÃ±¾º¯Êı
	huifu_default_set_flag = 0;///
	AllScreen_Show_Flag = 0;
	show_flash_flag = 0;/////¹Øµô·çËÙ×Ô¶¯µÄÉÁË¸
}


void show_all_screen(void)///ÆÁÄ»ËùÓĞ¶«Î÷¶¼ÏÔÊ¾
{
	display_show_char(0, 32);
	display_show_char(1, 32);
	display_show_char(2, 32);
	display_show_char(3, 32);
}

void test_lcd(void)
{
	// display_show_char(0,0x10);//-
 
   // display_show_char(2,i);
	
	//for(i = 0; i < 11; i++)
	{
		//display_show_char(2,i);
	}
	/*
	for(i = 11; i < 19; i++)
	{
    	display_show_char(0,i);
		Delay();
	}
   	*/
   //	for(i = 11; i < 19; i++)
   	//{
   		//display_show_char(0,i);
		//Delay();
   //	}
	display_show_char(0,27);
	//display_show_char(3,21);
	//display_clear();
	
	
	
	//lcd_power_off();
}


void num_flash(u8 data)////ÓÃÀ´ÈÃÁ½¸ö¶ÏÂë¹ÜÉÁË¸ÏÔÊ¾µÄº¯ÊınumÊÇ¿ØÖÆÏÔÊ¾»¹ÊÇÇå³ı
{
	
	if(Clean_Show_Flag==0)///ÏÔÊ¾Êı×Ö
	{
		show_temp(data);
		Clean_Show_Flag = 1;
	}
	else////ÇåµôÊı×Ö
	{
		display_clear_char(1);
		display_clear_char(2);
		Clean_Show_Flag = 0;
	}
	Num_Flash_Time = Num_Flash_BASE;
}

void turn_on_num_flash(void)////¿ªÆôflashÏÔÊ¾
{
	Num_Flash_On_Flag = 1;
	Num_Flash_Time = Num_Flash_BASE;
}

void turn_off_num_flash(void)
{
	Num_Flash_On_Flag = 0;
	Num_Flash_Time = 0;
}

void show_scan(void)
{
	if(power_key_count == 1)///power on
	{
		lcd_on();
		Reg_Data.all_statues_data |= 0x80;///bit7¿ª»úÎ»ÖÃ1
		Reg_Data.mPower_On_Off_Ctr = 1;
		power_key_count++;
	}
	else if(power_key_count == 3)//off
	{
		lcd_off(0);
		Reg_Data.all_statues_data &= (~0x80);///bit7¿ª»úÎ»ÖÃ0
		power_key_count = 0;
		Reg_Data.mPower_On_Off_Ctr = 0;
	}
	////////·çËÙ
	if(wind_key_count == 1)///Á½¸ñ·çËÙ
	{
		if(lcd_close_flag == 0)
			show_flash_num(0);
		show_flash_flag = 0;//¹Øµô×Ô¶¯ÏÔÊ¾
		wind_key_count++;
		Reg_Data.all_statues_data &= (~0x03);//bit0bit1 00µÍ·ç
		Reg_Data.wind_data = 0x00;///00µÍ·ç
	}
	else if(wind_key_count == 3)/////4¸ñ·çËÙ
	{
		if(lcd_close_flag == 0)
			show_flash_num(1);
		show_flash_flag = 0;//¹Øµô×Ô¶¯ÏÔÊ¾
		wind_key_count++;

		//Reg_Data.all_statues_data &= (~0x02);//bit0bit1 01ÖĞ·ç
		Reg_Data.all_statues_data |= 0x01;//bit0bit1 01ÖĞ·ç
		Reg_Data.all_statues_data &= (~0x02);//bit1Îª0
		Reg_Data.wind_data = 0x01;///00ÖĞ·ç
	}
	else if(wind_key_count == 5)////6¸ñ·çËÙ
	{
		if(lcd_close_flag == 0)
			show_flash_num(2);
		show_flash_flag = 0;//¹Øµô×Ô¶¯ÏÔÊ¾
		wind_key_count++;

		Reg_Data.all_statues_data &= (~0x01);//bit0bit1 02¸ß·ç
		Reg_Data.all_statues_data |= 0x02;///bit2 Îª1
		Reg_Data.wind_data = 0x02;///00¸ß·ç
	}
	else if(wind_key_count == 7)//¹Ø±Õ
	{
		//display_show_char(0, 18);///ÏÈÇåµô×Ô¶¯
		//show_flash_flag = 0;//¹Øµô×Ô¶¯ÏÔÊ¾
		//wind_key_count++;

		show_flash_flag = 1;//¿ªÆô×Ô¶¯
		wind_key_count = 0;
		flash_index = 27;//////µ±ÓÉ6µµ»»µ½×Ô¶¯µ²Ê±Òª´ÓĞÂ¿ªÊ¼£¬²»ÖÃ27£¬Ôò»á¼ÇÒäÉÏ´ÎÍ¦×¡µÄÎ»ÖÃ 

		Reg_Data.all_statues_data |= 0x03;//bit0bit1 03×Ô¶¯·ç
		Reg_Data.wind_data = 0x04;///04×Ô¶¯
	}
	//else if(wind_key_count == 9)
	//{
		//show_flash_flag = 1;//¿ªÆô×Ô¶¯
		//wind_key_count = 0;
	//}

	/////////////Ñ©»¨ºÍ¹âÑ¡Ôñ
	if(sun_key_count == 1)//ÏÔÊ¾*
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 25);///
		}
		wind_sun_slecet = 1;//ËµÃ÷ÏÔÊ¾Ñ©»¨
		sun_key_count++;

		Reg_Data.all_statues_data |= 0x04;///bit2 0¹Ø1¿ª
		Reg_Data.all_statues_data &= (~0x08);//bit3 0
		Reg_Data.all_statues_data &= (~0x30);///bit4 bit5 00Àä·ç
		Reg_Data.mode_data = 0x00;//00ÖÆÀä
	}
	else if(sun_key_count == 3)///ÏÔÊ¾Ñô¹â
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 26);
		}
		wind_sun_slecet = 2;//ËµÃ÷ÏÔÊ¾Ñô¹â
		sun_key_count++;

		Reg_Data.all_statues_data |= 0x08;///bit3 0¹Ø1¿ª
		Reg_Data.all_statues_data &= (~0x04);//bit2 0
		Reg_Data.all_statues_data |= 0x10;///bit4 bit5 01ÈÈ·ç
		Reg_Data.all_statues_data &= (~0x20);//bit5Î»0
		Reg_Data.mode_data = 0x01;//01ÖÆÈÈ
	}
	else if(sun_key_count == 5)///¹Øµô
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 24);
		}
		wind_sun_slecet = 0;//Ê²Ã´Ò²²»ÏÔÊ¾
		sun_key_count = 0;

		Reg_Data.all_statues_data &= (~0x0c);///bit3 bit2 0¹ØÀäÈÈ¶¼¹Øµô
		Reg_Data.all_statues_data |= 0x20;///bit4 bit5 02Í¨·ç
		Reg_Data.all_statues_data &= (~0x10);//bit4 0
		Reg_Data.mode_data = 0x02;//02Í¨·ç
	}
	////ÎÂ¶È¼Ó
	if(add_key_count == 1)
	{
		add_key_count = 0;
		if(lcd_close_flag == 0)
			show_temp(Temperature_Count);
	}

	if(sub_key_count == 1)
	{
		sub_key_count = 0;
		if(lcd_close_flag == 0)
			show_temp(Temperature_Count);
	}
	
}

////////////
