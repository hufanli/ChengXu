#include "bsp_uart.h"
#include "myinclude.h"
#include "stm8s.h"
#include "stm8s_uart2.h"



All_Reg_T Reg_Data;

//Rcv_Send_Data_T Rcv_Send_Data;
u8 Rcv_Current_Count = 0;////是来在接收数据的时候记录接收一帧的第几个位置
u8 Fram_Delay_Time = 0;///每一帧延时时间
u8 One_Fram_End_Flag = 0;//一帧数据传输结束标志

u8 Rcv_Data[Communicate_Data_Len] = {0};////接收数据缓冲器
u8 Rcv_Data_Len = 0;////接收数据长度

u8 Send_Data[Communicate_Data_Len] = {0};///发送缓冲器
u8 Send_Data_Len = 0;///发送长度

u8 Fram_Over_Delay_Time = 0;///一帧结束延时时间
u8 Fram_Over_Delay_Time_Flag = 0;////时间用完标志

u16 Save_All_Data_Time =(u16)Save_All_Data_Time_Base ;////定时保存所有寄存器值

const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

const unsigned  char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

unsigned short crc(unsigned char *puchMsg , unsigned short usDataLen)
{
   unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
   unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
   unsigned uIndex ; /* will index into CRC lookup table */
   
   while (usDataLen--)/* pass through message buffer */
   {
     uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC */
     uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
     uchCRCLo = auchCRCLo[uIndex] ;
   }
   return (uchCRCHi << 8 | uchCRCLo) ;
}

//modbus校验
int crc_check( unsigned char *buf,int lenth)
{
   unsigned int tmp,tmp1;
   
  tmp=crc(buf,lenth-2);
  tmp1= *(buf+lenth-2)*256; 
  tmp1=tmp1+ *(buf+lenth-1);
  if (tmp == tmp1 )return 0;
  else return 1;;
}

void init_all_data(u8 num)///初始化所有临时变量num 0初始化，1长按初始化
{
	u8 temp[2];
	if(num == 0)
		load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);

	read_flash((u32)(FLASH_ADDR_BASE+Temp_JiaoZheng), temp, 2);//读取矫正温度
	if(temp[0] == 0)////正值
	{
		JiaoZheng_Temp = (s8)temp[1];
	}
	else ///负值
	{
		JiaoZheng_Temp = (s8)-temp[1];
	}
	//模式判断
	if(Save_Statues.sun_xue_slecet == 2)////雪花制冷
	{
	
		Reg_Data.all_statues_data |= 0x04;///bit2 0关1开
		Reg_Data.all_statues_data &= (~0x08);//bit3 0
		Reg_Data.all_statues_data &= (~0x30);///bit4 bit5 00冷风
		Reg_Data.mode_data = 0x00;//00制冷
	}
	else if(Save_Statues.sun_xue_slecet == 4)///阳光制热
	{
		

		Reg_Data.all_statues_data |= 0x08;///bit3 0关1开
		Reg_Data.all_statues_data &= (~0x04);//bit2 0
		Reg_Data.all_statues_data |= 0x10;///bit4 bit5 01热风
		Reg_Data.all_statues_data &= (~0x20);//bit5位0
		Reg_Data.mode_data = 0x01;//01制热

	}
	else if((Save_Statues.sun_xue_slecet == 6)||(Save_Statues.sun_xue_slecet ==0))///无显示
	{


		Reg_Data.all_statues_data &= (~0x0c);///bit3 bit2 0关冷热都关掉
		Reg_Data.all_statues_data |= 0x20;///bit4 bit5 02通风
		Reg_Data.all_statues_data &= (~0x10);//bit4 0
		Reg_Data.mode_data = 0x02;//02通风

	}
	
	
	//风速判断
	if((Save_Statues.wind_statues == 8)||(Save_Statues.wind_statues == 0))//动态显示风速
	{
		Reg_Data.wind_data = 0x04;///风速自动
		Reg_Data.all_statues_data |= 0x03;//自动模式bit0 bit1 11
		
	}
	else if(Save_Statues.wind_statues == 2)////2格风速
	{
		Reg_Data.wind_data = 0x00;///2格风速
		Reg_Data.all_statues_data &= (~0x03);//bit0-bit1 风速位，00低速
	}
	else if(Save_Statues.wind_statues == 4)////4格风速
	{
		Reg_Data.all_statues_data |= 0x01;//bit0bit1 01中风
		Reg_Data.all_statues_data &= (~0x02);//bit1为0
		Reg_Data.wind_data = 0x01;///00中风
	}
	else if(Save_Statues.wind_statues == 6)////6格风速
	{

		Reg_Data.all_statues_data &= (~0x01);//bit0bit1 02高风
		Reg_Data.all_statues_data |= 0x02;///bit2 为1
		Reg_Data.wind_data = 0x02;///00高风
	}

	Reg_Data.mCode_Set_Temp = Save_Statues.temperature_code;//制冷温度
	
	Reg_Data.mHot_Set_Temp =  Save_Statues.temperature_hode;//制热温度
	Reg_Data.New_SetWind_Temp = Save_Statues.temperature_wind;//风速温度
	Reg_Data.mRoom_Temp = RoomTemperature;///室内温度
	if(lcd_close_flag == 0)////显示开启的状态
	{
		Reg_Data.mPower_On_Off_Ctr = 0x01;//开机状态
	}
	else
	{
		Reg_Data.mPower_On_Off_Ctr = 0x00;//关机状态
	}

	Reg_Data.mTemp_Ctr_Addr = Save_Statues.can_id;
	
}


void save_all_data(void)////把所有的数据写到相应的flash位置
{
#if 1
	write_flash((u32)(FLASH_ADDR_BASE+All_Statues), &Reg_Data.all_statues_data, 1);///写所有状态
	write_flash((u32)(FLASH_ADDR_BASE+Set_Mode), &Reg_Data.mode_data, 1);////存储模式数据
	write_flash((u32)(FLASH_ADDR_BASE+Set_Wind), &Reg_Data.wind_data, 1);///存储风速数据
	Reg_Data.mRoom_Temp = RoomTemperature;///////保存室内温度 
	write_flash((u32)(FLASH_ADDR_BASE+Room_Temp), &Reg_Data.mRoom_Temp, 1);///存储房间温度
	write_flash((u32)(FLASH_ADDR_BASE+Code_Set_Temp), &Reg_Data.mCode_Set_Temp, 1);///制冷温度保存

	write_flash((u32)(FLASH_ADDR_BASE+New_Wind_Set_Temp), &Reg_Data.New_SetWind_Temp, 1);////风速温度
	write_flash((u32)(FLASH_ADDR_BASE+Hot_Set_Temp), &Reg_Data.mHot_Set_Temp, 1);///热风温度
	write_flash((u32)(FLASH_ADDR_BASE+Power_On_Off_Ctr), &Reg_Data.mPower_On_Off_Ctr, 1);///开关机状态
	write_flash((u32)(FLASH_ADDR_BASE+Temp_Ctr_Addr), &Reg_Data.mTemp_Ctr_Addr, 1);////温控器地址
#endif	

}


void uart_init(void)////串口初始化配置
{
	disableInterrupts();
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);      //USART2_TX-PD5
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);        //USART2_RX-PD6       
	//UART2初始化，STM8S105没有UART1
	UART2_DeInit();
	UART2_Cmd(ENABLE);
	UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	//UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	//UART2_IT_RXNE
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	enableInterrupts();
}


void rcv_ctl_rst(void)
{
  
    Rcv_Current_Count = 0;
    
    Fram_Delay_Time = 0x00;
   
	//Rcv_Send_Data.command = 0;
	//Rcv_Send_Data.data_len = 0;
	//memset(Rcv_Send_Data.data, 0, Communicate_Data_Len);

	Rcv_Data_Len = 0;
	memset(Rcv_Data, 0, Communicate_Data_Len);
	One_Fram_End_Flag = 0;
}

/*
void uart_rcv_handler()
{
	u8 rcv_char = 0;
	rcv_char = UART2_ReceiveData8();
	switch (Rcv_Current_Count)
	{
		case 0x00:
		{
			if(rcv_char == Reg_Data.mTemp_Ctr_Addr)///如果和我ID匹配则进入
			{
				Rcv_Send_Data.can_id = rcv_char;
				Fram_Delay_Time = (u8)Fram_Delay_Time_Base;
				Rcv_Current_Count=0x01;
			}
			else
			{
				rcv_ctl_rst();//复位所有数据
			}
			break;
		}
		case 0x01:////命令码数据
		{
			if(Rcv_Send_Data.can_id == Reg_Data.mTemp_Ctr_Addr)
			{
				Rcv_Send_Data.command = rcv_char;
				Fram_Delay_Time = (u8)Fram_Delay_Time_Base;
				
				Rcv_Current_Count = 0x02;
			}
			break;
		}
		case 0x02:///这个不做任何事情，因为我只用了一个字节的addr前面用来补0
		{
			if(Rcv_Send_Data.can_id == Reg_Data.mTemp_Ctr_Addr)
			{
				Rcv_Current_Count = 0x03;
				Fram_Delay_Time = (u8)Fram_Delay_Time_Base;
				
			}
			break;
		}
		case 0x03:
		{
			if(Rcv_Send_Data.can_id == Reg_Data.mTemp_Ctr_Addr)
			{
				Rcv_Send_Data.data_addr = rcv_char;
				Rcv_Current_Count = 0x04;
				Fram_Delay_Time = (u8)Fram_Delay_Time_Base;
			}
			break;
		}
		case 0x04:
		{
			
		}
		
	}
}
*/

void send_data(u8 ch)
{
	while(UART2_GetFlagStatus(UART2_FLAG_TC) != SET);
	UART2_SendData8(ch);
	
}
void uart_send_data(u8 *data, u8 len)///发送数据串
{
	u8 i = 0;
	for(i = 0; i < len; i++)
	{
		send_data(data[i]);
	}
}
void uart_rcv_handler2(void)///收数据
{
	Rcv_Data[Rcv_Data_Len] =  UART2_ReceiveData8();
	Rcv_Data_Len++;
	Fram_Delay_Time = (u8)Fram_Delay_Time_Base;
}

void Set_Report(u8 num)///设置返回的数据
{
	u16 crc_temp = 0;
	if(num == 0)
		Send_Data[0] = Rcv_Data[0];
	else
		Send_Data[0] = Rcv_Data[5];
	Send_Data[1] = Rcv_Data[1];
	Send_Data[2] = Rcv_Data[2];
	Send_Data[3] = Rcv_Data[3];
	Send_Data[4] = Rcv_Data[4];
	Send_Data[5] = Rcv_Data[5];
	crc_temp = crc(Send_Data, 6);
	Send_Data[6] = (crc_temp>>8) & 0xff;
	Send_Data[7] = crc_temp &0xff;
	Send_Data_Len = 8;
}

void uart_comm(void)
{
	u16 crc_temp = 0; 
	u8 temp[Communicate_Data_Len] = {0}, i = 0;
	if(Rcv_Data[0] == Reg_Data.mTemp_Ctr_Addr)///是我的ID号
	{
		if((Rcv_Data_Len >254)||(Rcv_Data_Len<8))
		{
			rcv_ctl_rst();
			
		}
		
		if(crc_check(Rcv_Data ,(int) Rcv_Data_Len))///crc校验错误
		{
			rcv_ctl_rst();
			
		}
		else///正确数据
		{
			switch (Rcv_Data[1])////命令码
			{
				case Read_zhiding_Addr:////读取指定地址数据
				{
					if((Rcv_Data[3] >= 0x20)&&(Rcv_Data[3] <= 0x4D))///可用的地址区域
					{
						
						read_flash((u32)(FLASH_ADDR_BASE+Rcv_Data[3]), temp, Rcv_Data[5]);////读取指定地址数据到 Send_Data
						
						Send_Data[0] = Rcv_Data[0];///返回ID号
						Send_Data[1] = Rcv_Data[1];////返回命令码
						Send_Data[2] = 0;//数据长度高字节
						Send_Data[3] = Rcv_Data[5]*2;///数据长度低字节
						for(i = 0; i < Rcv_Data[5]; i++)
						{
							Send_Data[4+2*i] = 0;
							Send_Data[4+2*i+1] = temp[i];
						}
						Send_Data_Len = Rcv_Data[5]*2+4+2;
						crc_temp = crc(Send_Data, Send_Data_Len-2);
						Send_Data[Send_Data_Len-2] = (crc_temp>>8) & 0xff;
						Send_Data[Send_Data_Len-1] = crc_temp &0xff;
						
						rcv_ctl_rst();
					}
					else ////超出了寄存器地址区
					{
						Send_Data[Send_Data_Len++] = Rcv_Data[0];///ID号
						
						Send_Data[Send_Data_Len++] = (u8)Read_Command_ERR;///命令码
					
						Send_Data[Send_Data_Len++] = Rcv_Data[3];///地址

						crc_temp = crc(Send_Data, Send_Data_Len);
						Send_Data[Send_Data_Len++] = (crc_temp>>8) & 0xff;
						Send_Data[Send_Data_Len++] = crc_temp &0xff;
						rcv_ctl_rst();
						
					}
					break;
				}

				case Set_zhiding_Addr://设定指定地址数据
				{
					if((Rcv_Data[3] > 0x20)&&(Rcv_Data[3] <= 0x4D))///可用的地址区域
					{
						
							switch (Rcv_Data[3])
							{
								case  (All_Statues):///状态寄存器
								{
									///Reg_Data.all_statues_data = Rcv_Data[5];//设置数据
									///状态寄存器预留设置
									break;
								}
									
								case  (Set_Mode):////设置模式
								{
									Reg_Data.mode_data = Rcv_Data[5];///数据重新设置值
									if(Reg_Data.mode_data == 0x00)///修改相应的状态寄存器位00制冷
									{
										Reg_Data.all_statues_data |= 0x04;///bit2 0关1开
										Reg_Data.all_statues_data &= (~0x08);//bit3 0
										Reg_Data.all_statues_data &= (~0x30);///bit4 bit5 00冷风
										if(lcd_close_flag == 0)///开着则显示，没有开着则不显示
										{
											display_show_char(3, 25);///在显示屏上显示雪花
											wind_sun_slecet = 1;
											sun_key_count = 1;
										}
										else///显示关闭着
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.sun_xue_slecet = 2;
											save_struct2();
										}

										
										
									}
									else if(Reg_Data.mode_data == 0x01)///修改相应的状态寄存器位01制热
									{
										Reg_Data.all_statues_data |= 0x08;///bit3 0关1开
										Reg_Data.all_statues_data &= (~0x04);//bit2 0
										Reg_Data.all_statues_data |= 0x10;///bit4 bit5 01热风
										Reg_Data.all_statues_data &= (~0x20);//bit5位0
										if(lcd_close_flag == 0)///开着则显示，没有开着则不显示
										{
											display_show_char(3, 26);////相应位置显示阳光
											wind_sun_slecet = 2;///恢复相应的模式
											sun_key_count = 3;////同时改变相应按键 的值以便下次查询显示时能够显示本次
										
										}
										else///显示关闭情况下
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.sun_xue_slecet = 4;
											save_struct2();
										}
										
									}
									else if(Reg_Data.mode_data == 0x02)///修改相应的状态寄存器位02 通风
									{
										Reg_Data.all_statues_data &= (~0x0c);///bit3 bit2 0关冷热都关掉
										Reg_Data.all_statues_data |= 0x20;///bit4 bit5 02通风
										Reg_Data.all_statues_data &= (~0x10);//bit4 0

										if(lcd_close_flag == 0)
										{
											display_show_char(3, 24);
											wind_sun_slecet = 0;
											sun_key_count =  5;
										}
                                        else///显示关闭情况下
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.sun_xue_slecet = 6;
											save_struct2();
										}
									}

									Set_Report(0);
									rcv_ctl_rst();
									break;
								}
								
								case  (Set_Wind):///设置风速
								{
									Reg_Data.wind_data = Rcv_Data[5];///数据重新设置值
									if(Reg_Data.wind_data == 0)//低风
									{
										Reg_Data.all_statues_data &= (~0x03);//bit0bit1 00低风
										if(lcd_close_flag == 0)
										{
											wind_key_count = 1;
										}
										else///显示关闭着
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.wind_statues = 2;
											save_struct2();
										}
									}
									else if(Reg_Data.wind_data == 0x01)//中速
									{
										Reg_Data.all_statues_data |= 0x01;//bit0bit1 01中风
										Reg_Data.all_statues_data &= (~0x02);//bit1为0
										if(lcd_close_flag == 0)
										{
											wind_key_count = 3;
										}
										else 
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.wind_statues = 4;
											save_struct2();
										}
									}
									else if(Reg_Data.wind_data == 0x02)///高速
									{
										Reg_Data.all_statues_data &= (~0x01);//bit0bit1 02高风
										Reg_Data.all_statues_data |= 0x02;///bit2 为1
										if(lcd_close_flag == 0)
										{
											wind_key_count = 5;
										}
										else 
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.wind_statues = 6;
											save_struct2();
										}
									}
									else if(Reg_Data.wind_data == 0x04)///风速自动
									{
										Reg_Data.all_statues_data |= 0x03;//bit0bit1 03自动风
										if(lcd_close_flag == 0)
										{
											wind_key_count = 7;
										}
										else
										{
											load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
											Save_Statues.wind_statues = 8;
											save_struct2();
										}
									}

									Set_Report(0);
									rcv_ctl_rst();
									break;
								}
								
								case  (Room_Temp):///这个不可以设置
								{
									break;
								}
								case  (Code_Set_Temp):
								{
									Reg_Data.mCode_Set_Temp = Rcv_Data[5];///数据重新设置值
									load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
									Save_Statues.temperature_code = Reg_Data.mCode_Set_Temp;
									save_struct2();
									Set_Report(0);
									rcv_ctl_rst();
									break;
								}
								
								case  (New_Wind_Set_Temp):///新风温度
								{
									Reg_Data.New_SetWind_Temp = Rcv_Data[5];///数据重新设置值
									load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
									Save_Statues.temperature_wind = Reg_Data.New_SetWind_Temp;
									save_struct2();
									Set_Report(0);
									rcv_ctl_rst();
									break;
								}
								
								case  (Hot_Set_Temp):///热风设置
								{
									Reg_Data.mHot_Set_Temp = Rcv_Data[5];///数据重新设置值
									load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
									Save_Statues.temperature_hode = Reg_Data.mHot_Set_Temp;
									save_struct2();
									Set_Report(0);
									rcv_ctl_rst();
									break;
								}

                                case   (Power_On_Off_Ctr) :
								{
									Reg_Data.mPower_On_Off_Ctr = Rcv_Data[5];
									
									if(Reg_Data.mPower_On_Off_Ctr == 0)
									{
										power_key_count = 3;//去关�
										lcd_close_flag = 1;
									}
									else////开机
									{
										power_key_count = 1;
										lcd_close_flag = 0;
                                        Bei_Guang_Time = (u32)BEI_GUANG_TIME_BASE;///只要有按键按下背光都亮
									}
									Set_Report(0);
									rcv_ctl_rst();
									break;
								}
								
                                case   (Temp_Ctr_Addr):
								{
									Reg_Data.mTemp_Ctr_Addr = Rcv_Data[5];
									load_struct((u32)(FLASH_ADDR_BASE+Set_Data_Offset),  (u8)Default_Data_Len);
									Save_Statues.can_id = Reg_Data.mTemp_Ctr_Addr;
									save_struct2();
									Set_Report(1);
									rcv_ctl_rst();
									break;
								}
																

								default:
									break;
							}
							rcv_ctl_rst();///// 本次收到的数据已经用过要清掉
						}
						else////超出了寄存器地址区
						{
							Send_Data[Send_Data_Len++] = Rcv_Data[0];///ID号
						
							Send_Data[Send_Data_Len++] = (u8)Read_Command_ERR;///命令码
					
							Send_Data[Send_Data_Len++] = Rcv_Data[3];///地址

							crc_temp = crc(Send_Data, Send_Data_Len);
							Send_Data[Send_Data_Len++] = (crc_temp>>8) & 0xff;
							Send_Data[Send_Data_Len++] = crc_temp &0xff;
							rcv_ctl_rst();
						}
					break;
				}

				default:
    				break;
			}
		}
	}
	else/// 不是我的ID号
	{	
        #if 0
		Send_Data[Send_Data_Len++] = Reg_Data.mTemp_Ctr_Addr;///ID号
						
		Send_Data[Send_Data_Len++] = (u8)Id_Num_Err;///命令码
		Send_Data[Send_Data_Len++] = Rcv_Data[3];///地址
		crc_temp = crc(Send_Data, Send_Data_Len);
		Send_Data[Send_Data_Len++] = (crc_temp>>8) & 0xff;
		Send_Data[Send_Data_Len++] = crc_temp &0xff;
        #endif
		rcv_ctl_rst();
        
	}

	
}

void Send_Handle(void)
{
	if(Send_Data_Len > 0)///有数据要发送
	{
		m485_C_Hight();//发送状态
		
		Fram_Over_Delay_Time = 12;
		if(Fram_Over_Delay_Time)
		{
			uart_send_data(Send_Data, Send_Data_Len);
		}
		Send_Data_Len = 0;
		
	}
	if(Fram_Over_Delay_Time_Flag == 1)////延时时间结束
	{
			m485_C_Low();///发送完就开启接收
			Fram_Over_Delay_Time_Flag = 0;
			Fram_Over_Delay_Time = 0;
            Send_Data_Len = 0;
	}
}
