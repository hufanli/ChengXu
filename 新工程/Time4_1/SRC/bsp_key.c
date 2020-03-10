#include "bsp_key.h"
#include "bsp_inc.h"
#include "myinclude.h"

u8 power_key_count = 0;///电源按钮
u8 wind_key_count = 0;///风速按钮按下次数
u8 sun_key_count = 0;//光照按钮按下次数
u8 add_key_count = 0;//加按键按下次数
u8 sub_key_count = 0;//减按键按下次数

u32 MKey_PullDown_Time = 0;////m按键长按判断
u8 MKey_PullDown_Flag = 0;////m按键长按标志，0没有按下，1位长按按下

u32 UpKey_PullDown_Time = 0;///上按键长按时间
u8  UpKey_PullDown_Flag = 0;///上按键长按按下标志0没有按下，1按下

u32 DownKey_PullDown_Time = 0;///下按键长按时间
u8 DownKey_PullDown_Flag = 0;////下按键长按下标志，0没有按下，1按下

u32 WindKey_PullDown_Time = 0;/////风速长按键按下
u8 WindKey_PullDown_Flag = 0;///风速长按键标志 0没有按下1按下
u8 WenduJiaoZhengJiLu = 0;////记录本次已经矫正了显示了，下次不需要再次显示

u8 key1_delay_time = 0;///按键1消抖延时时间
u8 key1_pulldown_flag = 0;///按下标志


void key_init(void)
{
	GPIO_Init(POWER_KEY, POWER_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(WIND_KEY, WIND_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SUN_KEY, SUN_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(ADD_KEY, ADD_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SUB_KEY, SUB_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);

	

	//GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_HIZ_FAST);
    
	GPIO_Init(m485_C_POART, m485_C_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);///485控制脚

    GPIO_Init(BEEP_PORT, BEEP_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);///初始化BEEP引脚
    GPIO_WriteLow(BEEP_PORT,BEEP_PIN);
	
}

void scan_key(void)
{	
	
	if(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS))///按下
	{
		Delay(30);
		
		if(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS))
		{
            //if(WindKey_PullDown_Flag == 0)////当风键是长按时禁止进入
            if((WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////当风键是长按时禁止进入
            {
                Beep_Open();
    			while(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS));
                           // Delay(60);
    			if(MKey_PullDown_Flag == 0)////不是长按键才能进入
    			{
    				power_key_count++;
    				//if(lcd_close_flag == 0)
    				{
    					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
    					lcd_power_on();
    				}
    			}
            }
			
		}
	}

	if(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS))///风按下
	{
		Delay(200);
        if(WindKey_PullDown_Flag == 0)
        {
		    if(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS))
    		{
    			if(lcd_close_flag == 1)
    			{
    				WindKey_PullDown_Time = (u32)WindKey_PullDown_TimeBase;
    			}
                else////开机情况下才能响beep
                {
                    WindKey_PullDown_Time = 0;
                    Beep_Open();
                }
    			while(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS)&&(!WindKey_PullDown_Flag));
    			//Delay(70);
    			if((MKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////不是长按键才能进入
    			{
    				
    				if(lcd_close_flag == 0)///开着显示屏才能进入
    				{
    				if(Show_Temp_Flash_Flag)///当闪烁时不去改变模式
    				{
    					Show_Temp_Flash_Flag = 0;///一旦按下本健则停止闪烁
    					show_temp(RoomTemperature);//这里是为了弥补当上面关掉闪烁标志，而这时恰好显示屏有没有显示则就可能显示不出来温度了
    				}
    				else
    				{
    					wind_key_count++;
    				}
    				if(lcd_close_flag == 0)
    				{
                                        if(WindKey_PullDown_Flag == 0)///修改于0813
                                        {
                                          Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
                                        }
                                        else
                                        {
                                          Bei_Guang_Time = 0;///
                                        }
    					lcd_power_on();
    				}
    				}
    				
    			}
    			else if(MKey_PullDown_Flag == 1)
    			{
    				//save_statuse();
    				MKey_PullDown_Flag = 0;///清掉以备下次再次长按的检测
    				MKey_PullDown_Time = 0;
    				lcd_off(0);
    				
    			}
    		}
        }
	}

	if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))///M按下
	{
		Delay(250);
		if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))
		{
			if(lcd_close_flag == 1)///开着显示时不能进行长按处理
			{
				MKey_PullDown_Time = (u32)MKey_PullDown_Time_Base;
			}
            else////开机情况下才能响beep
            {
                MKey_PullDown_Time = 0;
                Beep_Open();
            }
			while(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS)&&(!MKey_PullDown_Flag));
			
			
			//Delay(50);
			if((MKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))///为0才能进入，为1则说明是进行长按的配置
			{
				if(lcd_close_flag == 0)///只有显示屏开着才有效
				{
				if(Show_Temp_Flash_Flag)///当闪烁时不去改变模式
				{
					Show_Temp_Flash_Flag = 0;///一旦按下本健则停止闪烁
					show_temp(RoomTemperature);//这里是为了弥补当上面关掉闪烁标志，而这时恰好显示屏有没有显示则就可能显示不出来温度了
				}
				else////不闪烁的话改变模式
				{
					sun_key_count++;
				}

				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
					lcd_power_on();
				}
				}
				if(WindKey_PullDown_Flag == 1)
				{
					save_jiaozheng_temp();
					WindKey_PullDown_Flag = 0;
					WindKey_PullDown_Time = 0;
                    WenduJiaoZhengJiLu = 0;
					lcd_off(1);
				}
			}
			
		}
	}

	if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))///上按下
	{
		Delay(200);
		if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))
		{
			if(lcd_close_flag == 1)///开着显示时不能进行长按处理
			{
				UpKey_PullDown_Time = (u32)UpKey_PullDown_Time_Base;///让定时器进行长按判断
			}
            else////开机情况下才能响beep
            {
                UpKey_PullDown_Time = 0;
                Beep_Open();
            }
			while(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS)&&(!UpKey_PullDown_Flag));
                       // Delay(50);
            if(huifu_default_set_flag == 0)////这里的判断是防止当长按上键时却两次进入会使数字进入闪烁模式
            {
			if((MKey_PullDown_Flag == 0)&&(UpKey_PullDown_Flag == 0)&&(WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////如果不是M长按则进入
			{
				if(lcd_close_flag == 0)
				{
				if(wind_sun_slecet == 1)//选择雪花
				{
					add_key_count++;
					//Temperature_Count++;
					Save_Statues.temperature_code++;
					if(Save_Statues.temperature_code > Wendu_MaxSet)//温度最高设置
						Save_Statues.temperature_code = Wendu_MaxSet;
					Reg_Data.mCode_Set_Temp = Save_Statues.temperature_code;///制冷温度给寄存器变量
					Show_Temp_Flash_Flag = 1;//进行闪烁
					Temperature_Count = Save_Statues.temperature_code;//让系统去显示
				}
				else if(wind_sun_slecet == 2)//选择阳光
				{
					add_key_count++;
					Save_Statues.temperature_hode++;
					if(Save_Statues.temperature_hode > Wendu_MaxSet)//温度最高设置
						Save_Statues.temperature_hode = Wendu_MaxSet;
					Show_Temp_Flash_Flag = 1;//进行闪烁
					Reg_Data.mHot_Set_Temp = Save_Statues.temperature_hode;///把制热温度付给寄存器变量
					Temperature_Count = Save_Statues.temperature_hode;//让系统去显示
				}
				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
					lcd_power_on();
				}
				}
			}
			else if(MKey_PullDown_Flag == 1)//是M长按下
			{
				Save_Statues.can_id++;///如果是长按的状态下再按下上键则ID加1
				if(Save_Statues.can_id == 100)
				{
					Save_Statues.can_id = 0;
				}
				Reg_Data.mTemp_Ctr_Addr = Save_Statues.can_id;///把ID号给寄存器地址
			}
			else if(WindKey_PullDown_Flag == 1)//风长按下
			{
				if(JiaoZheng_Temp < 6)
				{
					JiaoZheng_Temp++;
                    WenduJiaoZhengJiLu = 0;
				}
			}
            }
			else
			{
				huifu_default_set_flag = 0;
			}
			//if(UpKey_PullDown_Flag == 1)///上键长按下
			//{
				//load_struct((u32)(FLASH_ADDR_BASE+Default_Set_Offset), Default_Data_Len);
			//}
		}
	}

	if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))///下按下
	{
		Delay(200);
		if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))
		{
			if(lcd_close_flag == 1)///开着显示时不能进行长按处理
			{
				DownKey_PullDown_Time = (u32)DownKey_PullDown_Time_Base;
			}
            else////开机情况下才能响beep
            {
                DownKey_PullDown_Time = 0;
                Beep_Open();
            }
			while(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS)&&(!DownKey_PullDown_Flag));
			//Delay(50);
			if(MKey_PullDown_Flag == 0&&(UpKey_PullDown_Flag == 0)&&(WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))
			{
				if(lcd_close_flag == 0)
				{
				if(wind_sun_slecet == 1)//选择雪花
				{
					sub_key_count++;
					//Temperature_Count++;
					Save_Statues.temperature_code--;
					if(Save_Statues.temperature_code < Wendu_MixSet)//温度最高设置
						Save_Statues.temperature_code = Wendu_MixSet;
					Show_Temp_Flash_Flag = 1;//进行闪烁
					Reg_Data.mCode_Set_Temp = Save_Statues.temperature_code;///制冷温度给寄存器变量
					Temperature_Count = Save_Statues.temperature_code;//让系统去显示
				}
				else if(wind_sun_slecet == 2)//选择阳光
				{
					sub_key_count++;
					Save_Statues.temperature_hode--;
					if(Save_Statues.temperature_hode< Wendu_MixSet)//温度最高设置
						Save_Statues.temperature_hode = Wendu_MixSet;
					Show_Temp_Flash_Flag = 1;//进行闪烁
					Reg_Data.mHot_Set_Temp = Save_Statues.temperature_hode;///把制热温度付给寄存器变量
					Temperature_Count = Save_Statues.temperature_hode;//让系统去显示
				}	
				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
					lcd_power_on();
				}
				}
			}
			else if(MKey_PullDown_Flag == 1)//是M长按下
			{
				if(Save_Statues.can_id != 0)///减到0则为0
				{
					Save_Statues.can_id--;
					Reg_Data.mTemp_Ctr_Addr = Save_Statues.can_id;///把ID号给寄存器地址
				}
				
			}
			else if(WindKey_PullDown_Flag == 1)
			{
				if(JiaoZheng_Temp != -6)
				{
					JiaoZheng_Temp--;
                    WenduJiaoZhengJiLu = 0;
				}
			}
		}
	}
		
}

