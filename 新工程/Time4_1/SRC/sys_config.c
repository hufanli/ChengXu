#include "sys_config.h"
#include "stm8s.h"
#include "myinclude.h"


u16 Beep_Delay_Time = 0;/////延时时间
u8 Use_Beep_Flag = 0;////1使用beep  0不使用beep
u8 Sys_Time = 0;
u8 AllScreen_Show_Open_Flag = 0;///用来开启全屏后结下来就不开了

void sys_clkconfig(void)
{
	 GPIO_Init(GPIOD,GPIO_PIN_0,GPIO_MODE_OUT_OD_LOW_FAST);
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_HSICmd(ENABLE);
	while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSI){}




 // CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
//  CLK_ClockSwitchCmd(ENABLE);
//  CLK_SYSCLKSourceSwitchCmd(ENABLE);
//  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
//  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI){}

}

void time1_config(void)
{
	
   // disableInterrupts();
	 /* Enable TIM3 CLK */
  	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE); 
  
  	/* Time base configuration */
 	// TIM1_TimeBaseInit(TIM1_Prescaler_64, TIM1_CounterMode_Up, 25000);//1uS*1000*10 =10mS
  	TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,10,10);
  	/* Clear TIM3 update flag */
  	TIM1_ClearFlag(TIM1_FLAG_UPDATE);
  
 	/* Enable update interrupt */
 	 TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
  
  	/* Enable TIM3 */ 
 	 TIM1_Cmd(ENABLE);


}




void time2_config()
{
  TIM2_DeInit();//reset
  TIM2_PrescalerConfig(TIM2_PRESCALER_16,TIM2_PSCRELOADMODE_UPDATE);//16分频成1MHZ，EGR不产生更新事件//
  
  TIM2_ITConfig( TIM2_IT_UPDATE , ENABLE); //使能中断
  
  TIM2_SetCounter(800);//装入初值 
 
  TIM2_Cmd(ENABLE);//计时器打开//
 
}


void time4_config(void)///15.625ms
{
	//默认CLK8分频，16M/8=2M
  //CLK->ICKR=0x01; //设置内部时钟晶振 0x01为打开ICKR寄存器中的HSIEN位  by SONG
  //设置定时器周期，定时周期=250*62.5=15625uS=15.625ms
  TIM4->ARR=250;//计数到128后产生溢出
  TIM4->PSCR=0x07;//128分频,即2M/128=16KHz(相当于计数器周期为62.5us）
  TIM4->IER=0X01;//启用TIM4更新中断标志位
  TIM4->EGR=0x01;//允许产生更新事件
  TIM4->CR1=0X01;//启动定时器
}

void systime_ctrl(void)
{
	if(Bei_Guang_Time != 0)
	{
		Bei_Guang_Time--;
		if(Bei_Guang_Time == 0)///关掉背光
		{
			lcd_power_off();
			Show_Temp_Flash_Flag = 0;///一旦背光关掉就关掉闪烁
			show_temp(RoomTemperature);//这里是为了弥补当上面关掉闪烁标志，而这时恰好显示屏有没有显示则就可能显示不出来温度了
		}
	}

	if(Flash_Delay_Time != 0)/// 风速动态显示延时时间
	{
		Flash_Delay_Time--;	
	}

	if(Show_Temp_Flash_Time != 0)///修改温度闪烁间隔时间
	{
		Show_Temp_Flash_Time--;
	}

	if(Num_Flash_Time != 0)
	{
		Num_Flash_Time--;
	}

	if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))////M按键按下
	{
		if(MKey_PullDown_Time != 0)
		{
			MKey_PullDown_Time--;
			if(MKey_PullDown_Time == 0)
			{
				MKey_PullDown_Flag = 1;//说明长按下了
			}
		}
	}

	if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))///上健按下
	{
		if(UpKey_PullDown_Time != 0)
		{
			UpKey_PullDown_Time--;
			if(UpKey_PullDown_Time == 0)
			{
				UpKey_PullDown_Flag = 1;
			}
		}
	}

	if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))///下键按下
	{
		if(DownKey_PullDown_Time != 0)
		{
			DownKey_PullDown_Time--;
			if(DownKey_PullDown_Time == 0)
			{
				DownKey_PullDown_Flag = 1;
			}
		}
	}

	if(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS))
	{
		if(WindKey_PullDown_Time != 0)
		{
			WindKey_PullDown_Time--;
			if(WindKey_PullDown_Time == 0)
			{
				WindKey_PullDown_Flag = 1;
                lcd_on();
                display_clear();
			}
		}
	}

	if(AllScreen_Show_DelayTime != 0)
	{
		AllScreen_Show_DelayTime--;
	}

	if(get_temp_time != 0)
	{
		get_temp_time--;
	}

}

void judge_num_flash(void)///判断是否数字闪烁
{
	if(Num_Flash_On_Flag)
	{
		num_flash(Num_Flash_Data);
	}
}


////长按键的处理长按M设置ID,长按上键是屏幕全显，长按下键是恢复默认设置
void long_key_operate(void)
{
	////设置ID
	u8 temp = 0;
	if(MKey_PullDown_Flag)///m键长按按下
	{
		if(peizhi_ht1621_init_on_flag == 0)
		{
			peizhi_ht1621_init(0);
			load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);///读取flash中默认的ID号
		}
		show_temp(Save_Statues.can_id);
	}

	if(UpKey_PullDown_Flag)////上键长按按键，恢复默认设置，
	{
		if(huifu_default_set_flag == 0)
		{
			peizhi_ht1621_init(1);
			load_struct((u32)(FLASH_ADDR_BASE+Default_Set_Offset), Default_Data_Len);
			save_struct2();
			init_show();
			//init_all_data(1);
			power_key_count++;///这个是因为长按时已经开启了，所有要加1，以便下次关键按下一次就行了
			UpKey_PullDown_Flag = 0;///恢复了默认设置后就关掉长按
			UpKey_PullDown_Time = 0;
		}
		
		
	}

	if(DownKey_PullDown_Flag)////下键长按按下
	{	if(AllScreen_Show_Flag == 0)
		{
			AllScreen_Show_DelayTime = (u16)AllScreen_Show_DelayTime_BASE;
			peizhi_ht1621_init(2);///显示屏初始化配置
		}
		if((AllScreen_Show_DelayTime != 0)&&(AllScreen_Show_Open_Flag == 0))///时间没有用完则一直显示
		{
            AllScreen_Show_Open_Flag = 1;
			show_all_screen();
		}
		else if(AllScreen_Show_DelayTime == 0)
		{
            AllScreen_Show_Open_Flag = 0;
			lcd_off(0);
			DownKey_PullDown_Flag = 0;
			DownKey_PullDown_Time = 0;
		}
	}

	if(WindKey_PullDown_Flag)////风速长按下
	{
        if(WenduJiaoZhengJiLu == 0)
        {
            WenduJiaoZhengJiLu = 1;
    		if(JiaoZheng_On_Flag == 0)
    		{
    			//peizhi_ht1621_init(3);
    			lcd_power_on();
                
    		}
            display_clear();
    		display_show_char(0, 18);///显示C
    		//display_clear_char(3);
    		if(JiaoZheng_Temp >= 0)
    		{
    			temp = (u8)JiaoZheng_Temp;
    			display_show_char(1, temp);
    			display_show_char(2, 10);
    		}
    		else
    		{
    			temp = (u8)abs(JiaoZheng_Temp);
    			
    			display_show_char(1, temp);
    			display_show_char(2, 33);
    		}
        }

	}
    
}


////beep 
void Beep_Open(void)
{
    #if 1
    if(Use_Beep_Flag == 1)
    {
        Beep_Delay_Time = (u16)BEEP_DELAY_TIME_BASE;
    
        BEEP_ON();
       
    }
    else
    {
    }
    #else
    BEEP_ON();
    Delay(8000);
    BEEP_OFF();
 #endif    
}

void Beep_Scan(void)////浏览beep是开着还是关着
{
    if((Beep_Delay_Time == 0)&&(Use_Beep_Flag == 1))
    {
        BEEP_OFF();
    }
    
}
