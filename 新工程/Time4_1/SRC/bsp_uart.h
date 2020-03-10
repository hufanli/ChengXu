#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "myinclude.h"

///////命令码
#define Read_zhiding_Addr 0x03//读取指定地址数据
#define Set_zhiding_Addr 0x06///设定指定地址数据
#define Read_Command_ERR 0x83///读取指令错误返回
#define Set_Command_ERR 0x86///设定指令错误返回

#define Id_Num_Err 0xff///不是本机ID号报错

#if 0
#define All_Statues 0x0030///所有功能的状态
#define Set_Mode    0x0031///设置模式寄存器
#define Set_Wind    0x0032//设定风速
#define Room_Temp   0x0033//室内温度
#define Code_Set_Temp 0x0034///制冷设定温度℃
#define New_Wind_Set_Temp 0x0035///新风设定温度
#define Hot_Set_Temp 0x0036///制热设定温度
#define Power_On_Off_Ctr 0x0037///开关机控制00关机，01开机
#define Temp_Ctr_Addr   0x0038////存储温控器的地址
#endif
#define All_Statues 0x0030///所有功能的状态
#define Set_Mode    0x0031///设置模式寄存器
#define Set_Wind    0x0032//设定风速
#define Room_Temp   0x0033//室内温度
#define Code_Set_Temp 0x0036///制冷设定温度℃
#define New_Wind_Set_Temp 0x004C///新风设定温度
#define Hot_Set_Temp 0x004D///制热设定温度
#define Power_On_Off_Ctr 0x0039///开关机控制00关机，01开机
#define Temp_Ctr_Addr   0x0020////存储温控器的地址


#define Communicate_Data_Len  200

#define m485_C_POART GPIOD
#define m485_C_PINS GPIO_PIN_7
#define m485_C_Hight() GPIO_WriteHigh(m485_C_POART, m485_C_PINS)
#define m485_C_Low()   GPIO_WriteLow(m485_C_POART, m485_C_PINS)

extern u8 Fram_Delay_Time;///每一帧延时时间

#define Fram_Delay_Time_Base 4////帧延时基数
#define Save_All_Data_Time_Base 50///500ms


typedef struct All_Reg/////所有寄存器的临时变量
{
	u8 all_statues_data;////所有状态临时变量，上位机读取时把此数据返回，当关机时存储起来，
	u8 mode_data;///模式数据00冷风，01制热，02通风
	u8 wind_data;//风速数据00低速，01中速，02高速
	u8 mRoom_Temp;///室内温度
	u8 mCode_Set_Temp;//制冷设置温度
	u8 New_SetWind_Temp;///新风设定温度
	u8 mHot_Set_Temp;///制热设定温度
	u8 mPower_On_Off_Ctr;//开关机数据值00关机，01开机
	u8 mTemp_Ctr_Addr;///温控器地址
		
}All_Reg_T;

/*
typedef struct Rcv_Send_Data
{
	u8 can_id;//////这个是传输数据的id号
	u8 command;///命令码
	u8 data_addr;///数据地址
	u8 data_len;
	u8 data[Communicate_Data_Len];///传输数据
}Rcv_Send_Data_T;
*/

extern All_Reg_T Reg_Data;

//extern Rcv_Send_Data_T Rcv_Send_Data;
extern u8 Rcv_Current_Count;////是来在接收数据的时候记录接收一帧的第几个位置
extern u8 One_Fram_End_Flag;//一帧数据传输结束标志
extern u8 Fram_Over_Delay_Time_Flag;////时间用完标志

extern u8 Fram_Over_Delay_Time;///一帧结束延时时间


extern u8 Rcv_Data[Communicate_Data_Len];////接收数据缓冲器
extern u8 Rcv_Data_Len;////接收数据长度

extern u8 Send_Data[Communicate_Data_Len];///发送缓冲器
extern u8 Send_Data_Len;///发送长度

extern u16 Save_All_Data_Time;////定时保存所有寄存器值


void save_all_data(void);////把所有的数据写到相应的flash位置
void init_all_data(u8 num);///初始化所有临时变量num 0初始化，1长按初始化
void uart_init(void);////串口初始化配置
int crc_check( unsigned char *buf,int lenth);
void uart_rcv_handler2(void);
void uart_comm(void);
void Send_Handle(void);

#endif
