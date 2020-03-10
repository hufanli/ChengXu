/*
�ļ�����  main.c
���뻷����IAR for STM8S
����ƽ̨��9mcu.com  stm8s������
����ʱ�䣺2015-3-13
��д�ߣ�  songhongmei
ʵ�ֹ��ܣ��ö�ʱ��TIM4��������ʱ��ͨ����ѯ��ʱ���Ƿ���������¼������ж϶�ʱ������ʱ�Ƿ������Ȼ������LED��˸��
*/
#include "stm8s.h"
#include "myinclude.h"
#include "bsp_ht1621b.h"
#include "stm8s_uart2.h"




u8 testbuf[8] = {0xaa, 0xBB, 0xCC, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa };

u8 tempbuf[8];


main()
{
  
  u32 i = 0;
  sys_clkconfig();

  disableInterrupts();

 	time4_config();
	time1_config();
    ///time2_config;
	key_init();

	adc_config();////adc ������
	enableInterrupts();//�����ж�

	lcd_on();
    
	lcd_off(1);
	display_clear();
   flash_set_init();
	flash_default_init();////Ĭ��flash�洢��������
	
	init_all_data(0);///��ʼ��������ʱ����
	uart_init();

 // write_flash(FLASH_ADDR_BASE,testbuf,3);
  //read_flash(FLASH_ADDR_BASE, tempbuf,3);
	m485_C_Low();///�ϵ�485���ڽ��յ�״̬
	Use_Beep_Flag = 0;////��beep
	while(1)
	{


         
		if((get_temp_time == 0)&&(Show_Temp_Flash_Flag==0)&&(MKey_PullDown_Flag == 0)&&(WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))
		{
			get_temp_time = (u16)Get_Temp_Time_Base;


           //show_temp(RoomTemperature);
            
			RoomTemperature = get_temp();
			
			if(JiaoZheng_Temp > 0)///�����¶ȴ���0
			{
				RoomTemperature = RoomTemperature+(u8)abs(JiaoZheng_Temp);
			}
			else
			{
				RoomTemperature = RoomTemperature - (u8)abs(JiaoZheng_Temp);
			}
			if(lcd_close_flag == 0)
			{
				show_temp(RoomTemperature);
			}
            
		}
       
        
		if(One_Fram_End_Flag == 1)///˵��һ֡��������
		{
			uart_comm();
		}
		Send_Handle();////��������
		if(Save_All_Data_Time == 0)////�洢���мĴ���
		{
			Save_All_Data_Time = (u16)Save_All_Data_Time_Base;
			save_all_data();
		}

		scan_key();
		show_scan();
		judge_num_flash();
		Beep_Scan();////�������ص�
		long_key_operate();////�������ļ��
		
		if(wind_sun_slecet&&(!Show_Temp_Flash_Time)&&Show_Temp_Flash_Flag)
		{
			i++;
			if(i > 6000)
				i = 0;
			if(i%2 == 0)
			{
				show_temp(Temperature_Count);
			}
			else
			{
				display_clear_char(1);
				display_clear_char(2);
			}
			Show_Temp_Flash_Time = (u32)SHOW_TEMP_FLASH_TIME_BASE;
		}
		
		
		if(show_flash_flag&&(!lcd_close_flag))
		{
			
			show_temp_flash2();
		}
		
	}
	
}
























//__interrupt void TIM4_UPD_OVF_IRQHandler(void);
/*{
 i++;
  TIM4->SR1=0X00;//��������ж�״̬�Ĵ���SR�е�UIF�������жϱ�־��λ
  if(i==250)
 {
    i=0;         //250*4000us=1s
    sec++;   //��++
  }
}*/


