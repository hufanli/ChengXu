#include "sys_config.h"
#include "stm8s.h"
#include "myinclude.h"


u16 Beep_Delay_Time = 0;/////��ʱʱ��
u8 Use_Beep_Flag = 0;////1ʹ��beep  0��ʹ��beep
u8 Sys_Time = 0;
u8 AllScreen_Show_Open_Flag = 0;///��������ȫ����������Ͳ�����

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
  TIM2_PrescalerConfig(TIM2_PRESCALER_16,TIM2_PSCRELOADMODE_UPDATE);//16��Ƶ��1MHZ��EGR�����������¼�//
  
  TIM2_ITConfig( TIM2_IT_UPDATE , ENABLE); //ʹ���ж�
  
  TIM2_SetCounter(800);//װ���ֵ 
 
  TIM2_Cmd(ENABLE);//��ʱ����//
 
}


void time4_config(void)///15.625ms
{
	//Ĭ��CLK8��Ƶ��16M/8=2M
  //CLK->ICKR=0x01; //�����ڲ�ʱ�Ӿ��� 0x01Ϊ��ICKR�Ĵ����е�HSIENλ  by SONG
  //���ö�ʱ�����ڣ���ʱ����=250*62.5=15625uS=15.625ms
  TIM4->ARR=250;//������128��������
  TIM4->PSCR=0x07;//128��Ƶ,��2M/128=16KHz(�൱�ڼ���������Ϊ62.5us��
  TIM4->IER=0X01;//����TIM4�����жϱ�־λ
  TIM4->EGR=0x01;//������������¼�
  TIM4->CR1=0X01;//������ʱ��
}

void systime_ctrl(void)
{
	if(Bei_Guang_Time != 0)
	{
		Bei_Guang_Time--;
		if(Bei_Guang_Time == 0)///�ص�����
		{
			lcd_power_off();
			Show_Temp_Flash_Flag = 0;///һ������ص��͹ص���˸
			show_temp(RoomTemperature);//������Ϊ���ֲ�������ص���˸��־������ʱǡ����ʾ����û����ʾ��Ϳ�����ʾ�������¶���
		}
	}

	if(Flash_Delay_Time != 0)/// ���ٶ�̬��ʾ��ʱʱ��
	{
		Flash_Delay_Time--;	
	}

	if(Show_Temp_Flash_Time != 0)///�޸��¶���˸���ʱ��
	{
		Show_Temp_Flash_Time--;
	}

	if(Num_Flash_Time != 0)
	{
		Num_Flash_Time--;
	}

	if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))////M��������
	{
		if(MKey_PullDown_Time != 0)
		{
			MKey_PullDown_Time--;
			if(MKey_PullDown_Time == 0)
			{
				MKey_PullDown_Flag = 1;//˵����������
			}
		}
	}

	if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))///�Ͻ�����
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

	if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))///�¼�����
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

void judge_num_flash(void)///�ж��Ƿ�������˸
{
	if(Num_Flash_On_Flag)
	{
		num_flash(Num_Flash_Data);
	}
}


////�������Ĵ�����M����ID,�����ϼ�����Ļȫ�ԣ������¼��ǻָ�Ĭ������
void long_key_operate(void)
{
	////����ID
	u8 temp = 0;
	if(MKey_PullDown_Flag)///m����������
	{
		if(peizhi_ht1621_init_on_flag == 0)
		{
			peizhi_ht1621_init(0);
			load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);///��ȡflash��Ĭ�ϵ�ID��
		}
		show_temp(Save_Statues.can_id);
	}

	if(UpKey_PullDown_Flag)////�ϼ������������ָ�Ĭ�����ã�
	{
		if(huifu_default_set_flag == 0)
		{
			peizhi_ht1621_init(1);
			load_struct((u32)(FLASH_ADDR_BASE+Default_Set_Offset), Default_Data_Len);
			save_struct2();
			init_show();
			//init_all_data(1);
			power_key_count++;///�������Ϊ����ʱ�Ѿ������ˣ�����Ҫ��1���Ա��´ιؼ�����һ�ξ�����
			UpKey_PullDown_Flag = 0;///�ָ���Ĭ�����ú�͹ص�����
			UpKey_PullDown_Time = 0;
		}
		
		
	}

	if(DownKey_PullDown_Flag)////�¼���������
	{	if(AllScreen_Show_Flag == 0)
		{
			AllScreen_Show_DelayTime = (u16)AllScreen_Show_DelayTime_BASE;
			peizhi_ht1621_init(2);///��ʾ����ʼ������
		}
		if((AllScreen_Show_DelayTime != 0)&&(AllScreen_Show_Open_Flag == 0))///ʱ��û��������һֱ��ʾ
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

	if(WindKey_PullDown_Flag)////���ٳ�����
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
    		display_show_char(0, 18);///��ʾC
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

void Beep_Scan(void)////���beep�ǿ��Ż��ǹ���
{
    if((Beep_Delay_Time == 0)&&(Use_Beep_Flag == 1))
    {
        BEEP_OFF();
    }
    
}
