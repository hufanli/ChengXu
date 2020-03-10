/*
文件名：  main.c
编译环境：IAR for STM8S
编译平台：9mcu.com  stm8s开发板
编译时间：2015-3-13
编写者：  songhongmei
实现功能：用定时器TIM4来进行延时，通过查询定时器是否产生更新事件，来判断定时器的延时是否结束。然后驱动LED闪烁。
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

	adc_config();////adc 的配置
	enableInterrupts();//打开总中断

	lcd_on();
    
	lcd_off(1);
	display_clear();
   flash_set_init();
	flash_default_init();////默认flash存储配置数据
	
	init_all_data(0);///初始化所有临时变量
	uart_init();

 // write_flash(FLASH_ADDR_BASE,testbuf,3);
  //read_flash(FLASH_ADDR_BASE, tempbuf,3);
	m485_C_Low();///上电485处在接收的状态
	Use_Beep_Flag = 0;////打开beep
	while(1)
	{


         
		if((get_temp_time == 0)&&(Show_Temp_Flash_Flag==0)&&(MKey_PullDown_Flag == 0)&&(WindKey_PullDown_Flag == 0)&&(DownKey_PullDown_Flag == 0))
		{
			get_temp_time = (u16)Get_Temp_Time_Base;


           //show_temp(RoomTemperature);
            
			RoomTemperature = get_temp();
			
			if(JiaoZheng_Temp > 0)///矫正温度大于0
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
       
        
		if(One_Fram_End_Flag == 1)///说明一帧数据收完
		{
			uart_comm();
		}
		Send_Handle();////发送数据
		if(Save_All_Data_Time == 0)////存储所有寄存器
		{
			Save_All_Data_Time = (u16)Save_All_Data_Time_Base;
			save_all_data();
		}

		scan_key();
		show_scan();
		judge_num_flash();
		Beep_Scan();////蜂鸣器关掉
		long_key_operate();////长按键的检测
		
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
  TIM4->SR1=0X00;//软件清零中断状态寄存器SR中的UIF（更新中断标志）位
  if(i==250)
 {
    i=0;         //250*4000us=1s
    sec++;   //秒++
  }
}*/


