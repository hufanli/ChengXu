#include "bsp_ht1621b.h"
#include "stm8s.h"
#include "myinclude.h"

u8 Temperature_Count = 23;///用来显示室温用
u8 RoomTemperature = 26;/////室内温度变量

u32 Bei_Guang_Time = (u32)0;
u32 Flash_Delay_Time = (u32)0;//flash延时
u8 wind_sun_slecet = 0;//0 没有选择，1选择雪花，2选择阳光
u16 Show_Temp_Flash_Time = 0;///当修改温度时闪烁间隔时间

u8 LCD_RAM[18] = {0};
u8 show_flash_flag = 0;///flash 显示标志
u8 lcd_close_flag = 1;//整个显示关掉标志
u8 Bg_Light_flag = 0;//背光开闭标志0闭1开
u8 Show_Temp_Flash_Flag = 0;///温度显示闪烁标志0不闪烁，1闪烁

u8 Num_Flash_On_Flag = 0;////开数码管闪烁标志
u8 Num_Flash_Time = 0;//数字闪烁间隔时间
u8 Clean_Show_Flag = 0;//清除显示标志
u8 Num_Flash_Data = 0;///用来显示的数值

u8 peizhi_ht1621_init_on_flag = 0;///配置1621显示屏开启标志
u8 huifu_default_set_flag = 0;////恢复默认的设置

u8 JiaoZheng_On_Flag = 0; ///矫正开启标志

u8 AllScreen_Show_Flag = 0;//全显标志
u16 AllScreen_Show_DelayTime = 0;///所有屏幕全显时间

u8 flash_index = 27;////进行闪烁的位置

Save_Statuse_T Save_Statues;//


//0    1    2    3     4     5   6     7    8     9    清楚af
const u8 NUM_LAB[35] = {0xaf, 0xa0, 0xcb, 0xe9, 0xe4, 0x6d, 0x6f, 0xa8, 0xef, 0xed, 0x00,//10
			  		//  -     --    ---   ----  ----- ------       auto  C		 
                       0x0a, 0x09, 0x18, 0x28, 0x48, 0x88, 0x0c, 0x08,//18
                      0x0b, 0x1b, 0x3b, 0x7b, 0xfb, //23  分别是2个横一次
                     //电源 雪花 阳光
                       0x20, 0x60, 0xa0,///26
                       0x0e,0x0f, 0x1f, 0x3f, 0x7f, 0xff,////32
						///-
						0x40, 0x4f};// 0x4f显示E

/*
//0    1    2    3     4     5   6     7    8     9    清楚
const u8 NUM_LAB[22] = {0x80, 0x06, 0xcb, 0x4f, 0x66, 0x6d, 0xed, 0x07, 0xef, 0x67, 0x00,
			  		//  -     --    ---   ----  ----- ------       auto  C		 
                       0x02, 0x01, 0x10, 0x20, 0x40, 0x80, 0x04, 0x08,
                     //电源 雪花 阳光
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

void load_struct(u32 addr, u8 len)///把从flash中读取的数据装载到结构体里面
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

void save_struct()/////把struct的数据存到flash
{
	u8 temp[Default_Data_Len] = {0};///6个状态
	Save_Statues.wind_statues = wind_key_count;
	Save_Statues.sun_xue_slecet = sun_key_count;//保存是加热还是制冷
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
	u8 temp[Default_Data_Len] = {0};///6个状态
	//Save_Statues.wind_statues = wind_key_count;
	//Save_Statues.sun_xue_slecet = sun_key_count;//保存是加热还是制冷
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
//函数名称：Ht1621Wr_Data()
//功 能：写数据函数,cnt为传送数据位数,数据传送为低位在前
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
//函数名称：void Ht1621WrOneData(uint8_t Addr,uint8_t Data)
//功能描述: HT1621在指定地址写入数据函数
//参数说明：Addr为写入初始地址，Data为写入数据
//说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
//-----------------------------------------------------------------------------------------
void Ht1621WrOneData(u8 Addr,u8 Data)
{    
  HT1621_CS_Low();     //HT1621_CS=0;
  Ht1621Wr_Data(0xa0,3);    //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  Ht1621Wr_Data(Data<<4,4); //写入数据
  HT1621_CS_Hight();   //HT1621_CS=1;
}

//-----------------------------------------------------------------------------------------
//函数名称：void Ht1621WrCmd(uint8_t Cmd)
//功能描述: HT1621命令写入函数
//参数说明：Cmd为写入命令数据
//说 明：写入命令标识位100
//-----------------------------------------------------------------------------------------
void Ht1621WrCmd(u8 Cmd)
{
  HT1621_CS_Low();     //HT1621_CS=0;
  //    _delay_us(50);
  Ht1621Wr_Data(0x80,4);    //写入命令标志100
  Ht1621Wr_Data(Cmd,8);     //写入命令数据
  HT1621_CS_Hight();   //HT1621_CS=1;
}

void lcd_power_on(void)//背光开
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

void lcd_power_off(void)//背光关
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
函数说明:在指定位置显示一个字符
参    数:
index:显示位置1-8
dat:显示字符
返 回 值:-
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
函数说明:在指定位置擦除一个字符
参    数:
index:擦除位置1-8
返 回 值:-
*/
void display_clear_char(u8 index)
{
  Ht1621_Write_X_number(index,0x0A);
  
 
}

/**
函数说明:清屏
参    数:-	
返 回 值:-
*/
void display_clear(void)
{
  u8 i=0;
  
  for(i=0;i<4;i++)
    display_clear_char(i);
}

void show_temp(u8 temp)///显示温度
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
        display_show_char(2,34);///显示E
    	display_show_char(1,0);///0
    }
	#endif
}

void show_temp_flash(void)///动态显示
{
	display_show_char(0, 17);//清除
	
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
	display_show_char(0, 17);//清除
}


void show_temp_flash2()///动态显示
{
	
	//display_show_char(0, 17);//清除
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
	display_show_char(0, 18);///先清掉自动
	if(num == 0)
		display_show_char(0, 19);////2
	if(num == 1)
		display_show_char(0, 21);////4
	if(num == 2)
		display_show_char(0, 23);////6
}

void init_show(void)///上电读取flash默认的数值进行显示
{
	load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);

	init_all_data(1);
	//模式判断
	if(Save_Statues.sun_xue_slecet == 2)////雪花制冷
	{
		display_show_char(3, 25);///
		wind_sun_slecet = 1;
		sun_key_count = 1;
	}
	else if(Save_Statues.sun_xue_slecet == 4)///阳光制热
	{
		display_show_char(3, 26);
		wind_sun_slecet = 2;
		sun_key_count = 3;
	}
	else if((Save_Statues.sun_xue_slecet == 6)||(Save_Statues.sun_xue_slecet ==0))///无显示
	{
		display_show_char(3, 24);
		wind_sun_slecet = 0;
		
		sun_key_count =  5;
	}

	////显示室内温度
	show_temp(RoomTemperature);
	//风速判断
	if((Save_Statues.wind_statues == 8)||(Save_Statues.wind_statues == 0))//动态显示风速
	{
		show_temp_flash2();///动态显示--
		show_flash_flag = 1;
		wind_key_count = 0;
	}
	else if(Save_Statues.wind_statues == 2)////2格风速
	{
		show_flash_num(0);////2格
		wind_key_count = 1;
	}
	else if(Save_Statues.wind_statues == 4)////4格风速
	{
		show_flash_num(1);////4格
		wind_key_count = 3;
	}
	else if(Save_Statues.wind_statues == 6)////2格风速
	{
		show_flash_num(2);////6格
		wind_key_count = 5;
		
	}


	/////要修改按键现在对应按的位置
	
	
	
	display_show_char(0,18);///显示温度
	//show_temp_flash();///动态显示--
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
  	Ht1621WrCmd(RC256); //使用内部振荡器
  	Ht1621WrCmd(SYSDIS);  //关振系统荡器和LCD偏压发生器
  	Ht1621WrCmd(WDTDIS1); //禁止看门狗
  	Ht1621WrCmd(SYSEN); //打开系统振荡器
  	Ht1621WrCmd(LCDON); //打开LCD偏压

    
	HT1621_CS_Low();
	Ht1621Wr_Data(0xa0,3); //写入数据标志101
  	Ht1621Wr_Data(0x00<<2,6); //写入地址数据
	for (i=0;i<18;i++)
  	{
    	Ht1621Wr_Data(0x00,8); //写入数据
  	}
  	HT1621_CS_Hight();//HT1621_CS=1;  
	display_clear();///清屏
	init_show();////初始化显示重新把结构体中的数据进行显示
	//show_flash_flag = 1;
	  
}

void peizhi_ht1621_init(u8 num)///长按配置时要初始化显示屏的接口
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
  	Ht1621WrCmd(RC256); //使用内部振荡器
  	Ht1621WrCmd(SYSDIS);  //关振系统荡器和LCD偏压发生器
  	Ht1621WrCmd(WDTDIS1); //禁止看门狗
  	Ht1621WrCmd(SYSEN); //打开系统振荡器
  	Ht1621WrCmd(LCDON); //打开LCD偏压

	HT1621_CS_Low();
	Ht1621Wr_Data(0xa0,3); //写入数据标志101
  	Ht1621Wr_Data(0x00<<2,6); //写入地址数据
	for (i=0;i<18;i++)
  	{
    	Ht1621Wr_Data(0x00,8); //写入数据
  	}
  	HT1621_CS_Hight();//HT1621_CS=1;  
  	if(num == 0)
  	{
  		peizhi_ht1621_init_on_flag = 1;///说明开启了
  		lcd_close_flag = 0;////说明屏幕开启
  	}
	else if(num == 1)////恢复默认
	{
		huifu_default_set_flag = 1;///
		lcd_close_flag = 0;////说明屏幕开启
	}
	else if(num == 2)///全显
	{
		AllScreen_Show_Flag = 1;
		lcd_close_flag = 0;////说明屏幕开启
	}
	else if(num == 3)
	{
		JiaoZheng_On_Flag = 1;
		lcd_close_flag = 1;////说明屏幕开启
	}
	
	lcd_close_flag = 0;////说明屏幕开启
	display_clear();///清屏
}


void lcd_on(void)
{
	ht1621_init();
	lcd_close_flag = 0;
}

void lcd_off(u8 num)
{
	//u8 temp[20] = {0};
	
	display_clear();///清�
	Ht1621WrCmd(LCDOFF);
	HT1621_CS_Hight();
	HT1621_RD_Low();
	HT1621_WR_Low();
	HT1621_DT_Low();

	lcd_power_off();
	/*
	Save_Statues.wind_statues = wind_key_count;
	Save_Statues.sun_xue_slecet = sun_key_count;//保存是加热还是制冷
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
	Bei_Guang_Time = 0;//清零防止自启动
	Show_Temp_Flash_Flag = 0;//关掉闪烁标志
	peizhi_ht1621_init_on_flag = 0;///配置的关闭也调用本函数
	huifu_default_set_flag = 0;///
	AllScreen_Show_Flag = 0;
	show_flash_flag = 0;/////关掉风速自动的闪烁
}


void show_all_screen(void)///屏幕所有东西都显示
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


void num_flash(u8 data)////用来让两个断码管闪烁显示的函数num是控制显示还是清除
{
	
	if(Clean_Show_Flag==0)///显示数字
	{
		show_temp(data);
		Clean_Show_Flag = 1;
	}
	else////清掉数字
	{
		display_clear_char(1);
		display_clear_char(2);
		Clean_Show_Flag = 0;
	}
	Num_Flash_Time = Num_Flash_BASE;
}

void turn_on_num_flash(void)////开启flash显示
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
		Reg_Data.all_statues_data |= 0x80;///bit7开机位置1
		Reg_Data.mPower_On_Off_Ctr = 1;
		power_key_count++;
	}
	else if(power_key_count == 3)//off
	{
		lcd_off(0);
		Reg_Data.all_statues_data &= (~0x80);///bit7开机位置0
		power_key_count = 0;
		Reg_Data.mPower_On_Off_Ctr = 0;
	}
	////////风速
	if(wind_key_count == 1)///两格风速
	{
		if(lcd_close_flag == 0)
			show_flash_num(0);
		show_flash_flag = 0;//关掉自动显示
		wind_key_count++;
		Reg_Data.all_statues_data &= (~0x03);//bit0bit1 00低风
		Reg_Data.wind_data = 0x00;///00低风
	}
	else if(wind_key_count == 3)/////4格风速
	{
		if(lcd_close_flag == 0)
			show_flash_num(1);
		show_flash_flag = 0;//关掉自动显示
		wind_key_count++;

		//Reg_Data.all_statues_data &= (~0x02);//bit0bit1 01中风
		Reg_Data.all_statues_data |= 0x01;//bit0bit1 01中风
		Reg_Data.all_statues_data &= (~0x02);//bit1为0
		Reg_Data.wind_data = 0x01;///00中风
	}
	else if(wind_key_count == 5)////6格风速
	{
		if(lcd_close_flag == 0)
			show_flash_num(2);
		show_flash_flag = 0;//关掉自动显示
		wind_key_count++;

		Reg_Data.all_statues_data &= (~0x01);//bit0bit1 02高风
		Reg_Data.all_statues_data |= 0x02;///bit2 为1
		Reg_Data.wind_data = 0x02;///00高风
	}
	else if(wind_key_count == 7)//关闭
	{
		//display_show_char(0, 18);///先清掉自动
		//show_flash_flag = 0;//关掉自动显示
		//wind_key_count++;

		show_flash_flag = 1;//开启自动
		wind_key_count = 0;
		flash_index = 27;//////当由6档换到自动挡时要从新开始，不置27，则会记忆上次挺住的位置 

		Reg_Data.all_statues_data |= 0x03;//bit0bit1 03自动风
		Reg_Data.wind_data = 0x04;///04自动
	}
	//else if(wind_key_count == 9)
	//{
		//show_flash_flag = 1;//开启自动
		//wind_key_count = 0;
	//}

	/////////////雪花和光选择
	if(sun_key_count == 1)//显示*
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 25);///
		}
		wind_sun_slecet = 1;//说明显示雪花
		sun_key_count++;

		Reg_Data.all_statues_data |= 0x04;///bit2 0关1开
		Reg_Data.all_statues_data &= (~0x08);//bit3 0
		Reg_Data.all_statues_data &= (~0x30);///bit4 bit5 00冷风
		Reg_Data.mode_data = 0x00;//00制冷
	}
	else if(sun_key_count == 3)///显示阳光
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 26);
		}
		wind_sun_slecet = 2;//说明显示阳光
		sun_key_count++;

		Reg_Data.all_statues_data |= 0x08;///bit3 0关1开
		Reg_Data.all_statues_data &= (~0x04);//bit2 0
		Reg_Data.all_statues_data |= 0x10;///bit4 bit5 01热风
		Reg_Data.all_statues_data &= (~0x20);//bit5位0
		Reg_Data.mode_data = 0x01;//01制热
	}
	else if(sun_key_count == 5)///关掉
	{
		if(lcd_close_flag == 0)
		{
			display_show_char(3, 24);
		}
		wind_sun_slecet = 0;//什么也不显示
		sun_key_count = 0;

		Reg_Data.all_statues_data &= (~0x0c);///bit3 bit2 0关冷热都关掉
		Reg_Data.all_statues_data |= 0x20;///bit4 bit5 02通风
		Reg_Data.all_statues_data &= (~0x10);//bit4 0
		Reg_Data.mode_data = 0x02;//02通风
	}
	////温度加
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
