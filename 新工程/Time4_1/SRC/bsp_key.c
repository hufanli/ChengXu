#include "bsp_key.h"
#include "bsp_inc.h"
#include "myinclude.h"

u8 power_key_count = 0;///��Դ��ť
u8 wind_key_count = 0;///���ٰ�ť���´���
u8 sun_key_count = 0;//���հ�ť���´���
u8 add_key_count = 0;//�Ӱ������´���
u8 sub_key_count = 0;//���������´���

u32 MKey_PullDown_Time = 0;////m���������ж�
u8 MKey_PullDown_Flag = 0;////m����������־��0û�а��£�1λ��������

u32 UpKey_PullDown_Time = 0;///�ϰ�������ʱ��
u8  UpKey_PullDown_Flag = 0;///�ϰ����������±�־0û�а��£�1����

u32 DownKey_PullDown_Time = 0;///�°�������ʱ��
u8 DownKey_PullDown_Flag = 0;////�°��������±�־��0û�а��£�1����

u32 WindKey_PullDown_Time = 0;/////���ٳ���������
u8 WindKey_PullDown_Flag = 0;///���ٳ�������־ 0û�а���1����
u8 WenduJiaoZhengJiLu = 0;////��¼�����Ѿ���������ʾ�ˣ��´β���Ҫ�ٴ���ʾ

u8 key1_delay_time = 0;///����1������ʱʱ��
u8 key1_pulldown_flag = 0;///���±�־


void key_init(void)
{
	GPIO_Init(POWER_KEY, POWER_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(WIND_KEY, WIND_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SUN_KEY, SUN_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(ADD_KEY, ADD_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SUB_KEY, SUB_KEY_PINS, GPIO_MODE_IN_PU_NO_IT);

	

	//GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(BLIGHT_PORT, BLIGHT_PINS, GPIO_MODE_OUT_OD_HIZ_FAST);
    
	GPIO_Init(m485_C_POART, m485_C_PINS, GPIO_MODE_OUT_PP_HIGH_FAST);///485���ƽ�

    GPIO_Init(BEEP_PORT, BEEP_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);///��ʼ��BEEP����
    GPIO_WriteLow(BEEP_PORT,BEEP_PIN);
	
}

void scan_key(void)
{	
	
	if(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS))///����
	{
		Delay(30);
		
		if(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS))
		{
            //if(WindKey_PullDown_Flag == 0)////������ǳ���ʱ��ֹ����
            if((WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////������ǳ���ʱ��ֹ����
            {
                Beep_Open();
    			while(GPIO_ReadInputPin(POWER_KEY, POWER_KEY_PINS));
                           // Delay(60);
    			if(MKey_PullDown_Flag == 0)////���ǳ��������ܽ���
    			{
    				power_key_count++;
    				//if(lcd_close_flag == 0)
    				{
    					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///ֻҪ�а������±��ⶼ��
    					lcd_power_on();
    				}
    			}
            }
			
		}
	}

	if(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS))///�簴��
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
                else////��������²�����beep
                {
                    WindKey_PullDown_Time = 0;
                    Beep_Open();
                }
    			while(GPIO_ReadInputPin(WIND_KEY, WIND_KEY_PINS)&&(!WindKey_PullDown_Flag));
    			//Delay(70);
    			if((MKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////���ǳ��������ܽ���
    			{
    				
    				if(lcd_close_flag == 0)///������ʾ�����ܽ���
    				{
    				if(Show_Temp_Flash_Flag)///����˸ʱ��ȥ�ı�ģʽ
    				{
    					Show_Temp_Flash_Flag = 0;///һ�����±�����ֹͣ��˸
    					show_temp(RoomTemperature);//������Ϊ���ֲ�������ص���˸��־������ʱǡ����ʾ����û����ʾ��Ϳ�����ʾ�������¶���
    				}
    				else
    				{
    					wind_key_count++;
    				}
    				if(lcd_close_flag == 0)
    				{
                                        if(WindKey_PullDown_Flag == 0)///�޸���0813
                                        {
                                          Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///ֻҪ�а������±��ⶼ��
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
    				MKey_PullDown_Flag = 0;///����Ա��´��ٴγ����ļ��
    				MKey_PullDown_Time = 0;
    				lcd_off(0);
    				
    			}
    		}
        }
	}

	if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))///M����
	{
		Delay(250);
		if(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS))
		{
			if(lcd_close_flag == 1)///������ʾʱ���ܽ��г�������
			{
				MKey_PullDown_Time = (u32)MKey_PullDown_Time_Base;
			}
            else////��������²�����beep
            {
                MKey_PullDown_Time = 0;
                Beep_Open();
            }
			while(GPIO_ReadInputPin(SUN_KEY, SUN_KEY_PINS)&&(!MKey_PullDown_Flag));
			
			
			//Delay(50);
			if((MKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))///Ϊ0���ܽ��룬Ϊ1��˵���ǽ��г���������
			{
				if(lcd_close_flag == 0)///ֻ����ʾ�����Ų���Ч
				{
				if(Show_Temp_Flash_Flag)///����˸ʱ��ȥ�ı�ģʽ
				{
					Show_Temp_Flash_Flag = 0;///һ�����±�����ֹͣ��˸
					show_temp(RoomTemperature);//������Ϊ���ֲ�������ص���˸��־������ʱǡ����ʾ����û����ʾ��Ϳ�����ʾ�������¶���
				}
				else////����˸�Ļ��ı�ģʽ
				{
					sun_key_count++;
				}

				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///ֻҪ�а������±��ⶼ��
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

	if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))///�ϰ���
	{
		Delay(200);
		if(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS))
		{
			if(lcd_close_flag == 1)///������ʾʱ���ܽ��г�������
			{
				UpKey_PullDown_Time = (u32)UpKey_PullDown_Time_Base;///�ö�ʱ�����г����ж�
			}
            else////��������²�����beep
            {
                UpKey_PullDown_Time = 0;
                Beep_Open();
            }
			while(GPIO_ReadInputPin(ADD_KEY, ADD_KEY_PINS)&&(!UpKey_PullDown_Flag));
                       // Delay(50);
            if(huifu_default_set_flag == 0)////������ж��Ƿ�ֹ�������ϼ�ʱȴ���ν����ʹ���ֽ�����˸ģʽ
            {
			if((MKey_PullDown_Flag == 0)&&(UpKey_PullDown_Flag == 0)&&(WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))////�������M���������
			{
				if(lcd_close_flag == 0)
				{
				if(wind_sun_slecet == 1)//ѡ��ѩ��
				{
					add_key_count++;
					//Temperature_Count++;
					Save_Statues.temperature_code++;
					if(Save_Statues.temperature_code > Wendu_MaxSet)//�¶��������
						Save_Statues.temperature_code = Wendu_MaxSet;
					Reg_Data.mCode_Set_Temp = Save_Statues.temperature_code;///�����¶ȸ��Ĵ�������
					Show_Temp_Flash_Flag = 1;//������˸
					Temperature_Count = Save_Statues.temperature_code;//��ϵͳȥ��ʾ
				}
				else if(wind_sun_slecet == 2)//ѡ������
				{
					add_key_count++;
					Save_Statues.temperature_hode++;
					if(Save_Statues.temperature_hode > Wendu_MaxSet)//�¶��������
						Save_Statues.temperature_hode = Wendu_MaxSet;
					Show_Temp_Flash_Flag = 1;//������˸
					Reg_Data.mHot_Set_Temp = Save_Statues.temperature_hode;///�������¶ȸ����Ĵ�������
					Temperature_Count = Save_Statues.temperature_hode;//��ϵͳȥ��ʾ
				}
				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///ֻҪ�а������±��ⶼ��
					lcd_power_on();
				}
				}
			}
			else if(MKey_PullDown_Flag == 1)//��M������
			{
				Save_Statues.can_id++;///����ǳ�����״̬���ٰ����ϼ���ID��1
				if(Save_Statues.can_id == 100)
				{
					Save_Statues.can_id = 0;
				}
				Reg_Data.mTemp_Ctr_Addr = Save_Statues.can_id;///��ID�Ÿ��Ĵ�����ַ
			}
			else if(WindKey_PullDown_Flag == 1)//�糤����
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
			//if(UpKey_PullDown_Flag == 1)///�ϼ�������
			//{
				//load_struct((u32)(FLASH_ADDR_BASE+Default_Set_Offset), Default_Data_Len);
			//}
		}
	}

	if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))///�°���
	{
		Delay(200);
		if(GPIO_ReadInputPin(SUB_KEY, SUB_KEY_PINS))
		{
			if(lcd_close_flag == 1)///������ʾʱ���ܽ��г�������
			{
				DownKey_PullDown_Time = (u32)DownKey_PullDown_Time_Base;
			}
            else////��������²�����beep
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
				if(wind_sun_slecet == 1)//ѡ��ѩ��
				{
					sub_key_count++;
					//Temperature_Count++;
					Save_Statues.temperature_code--;
					if(Save_Statues.temperature_code < Wendu_MixSet)//�¶��������
						Save_Statues.temperature_code = Wendu_MixSet;
					Show_Temp_Flash_Flag = 1;//������˸
					Reg_Data.mCode_Set_Temp = Save_Statues.temperature_code;///�����¶ȸ��Ĵ�������
					Temperature_Count = Save_Statues.temperature_code;//��ϵͳȥ��ʾ
				}
				else if(wind_sun_slecet == 2)//ѡ������
				{
					sub_key_count++;
					Save_Statues.temperature_hode--;
					if(Save_Statues.temperature_hode< Wendu_MixSet)//�¶��������
						Save_Statues.temperature_hode = Wendu_MixSet;
					Show_Temp_Flash_Flag = 1;//������˸
					Reg_Data.mHot_Set_Temp = Save_Statues.temperature_hode;///�������¶ȸ����Ĵ�������
					Temperature_Count = Save_Statues.temperature_hode;//��ϵͳȥ��ʾ
				}	
				if(lcd_close_flag == 0)
				{
					Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///ֻҪ�а������±��ⶼ��
					lcd_power_on();
				}
				}
			}
			else if(MKey_PullDown_Flag == 1)//��M������
			{
				if(Save_Statues.can_id != 0)///����0��Ϊ0
				{
					Save_Statues.can_id--;
					Reg_Data.mTemp_Ctr_Addr = Save_Statues.can_id;///��ID�Ÿ��Ĵ�����ַ
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

