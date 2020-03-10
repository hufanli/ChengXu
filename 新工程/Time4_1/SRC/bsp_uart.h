#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "myinclude.h"

///////������
#define Read_zhiding_Addr 0x03//��ȡָ����ַ����
#define Set_zhiding_Addr 0x06///�趨ָ����ַ����
#define Read_Command_ERR 0x83///��ȡָ����󷵻�
#define Set_Command_ERR 0x86///�趨ָ����󷵻�

#define Id_Num_Err 0xff///���Ǳ���ID�ű���

#if 0
#define All_Statues 0x0030///���й��ܵ�״̬
#define Set_Mode    0x0031///����ģʽ�Ĵ���
#define Set_Wind    0x0032//�趨����
#define Room_Temp   0x0033//�����¶�
#define Code_Set_Temp 0x0034///�����趨�¶ȡ�
#define New_Wind_Set_Temp 0x0035///�·��趨�¶�
#define Hot_Set_Temp 0x0036///�����趨�¶�
#define Power_On_Off_Ctr 0x0037///���ػ�����00�ػ���01����
#define Temp_Ctr_Addr   0x0038////�洢�¿����ĵ�ַ
#endif
#define All_Statues 0x0030///���й��ܵ�״̬
#define Set_Mode    0x0031///����ģʽ�Ĵ���
#define Set_Wind    0x0032//�趨����
#define Room_Temp   0x0033//�����¶�
#define Code_Set_Temp 0x0036///�����趨�¶ȡ�
#define New_Wind_Set_Temp 0x004C///�·��趨�¶�
#define Hot_Set_Temp 0x004D///�����趨�¶�
#define Power_On_Off_Ctr 0x0039///���ػ�����00�ػ���01����
#define Temp_Ctr_Addr   0x0020////�洢�¿����ĵ�ַ


#define Communicate_Data_Len  200

#define m485_C_POART GPIOD
#define m485_C_PINS GPIO_PIN_7
#define m485_C_Hight() GPIO_WriteHigh(m485_C_POART, m485_C_PINS)
#define m485_C_Low()   GPIO_WriteLow(m485_C_POART, m485_C_PINS)

extern u8 Fram_Delay_Time;///ÿһ֡��ʱʱ��

#define Fram_Delay_Time_Base 4////֡��ʱ����
#define Save_All_Data_Time_Base 50///500ms


typedef struct All_Reg/////���мĴ�������ʱ����
{
	u8 all_statues_data;////����״̬��ʱ��������λ����ȡʱ�Ѵ����ݷ��أ����ػ�ʱ�洢������
	u8 mode_data;///ģʽ����00��磬01���ȣ�02ͨ��
	u8 wind_data;//��������00���٣�01���٣�02����
	u8 mRoom_Temp;///�����¶�
	u8 mCode_Set_Temp;//���������¶�
	u8 New_SetWind_Temp;///�·��趨�¶�
	u8 mHot_Set_Temp;///�����趨�¶�
	u8 mPower_On_Off_Ctr;//���ػ�����ֵ00�ػ���01����
	u8 mTemp_Ctr_Addr;///�¿�����ַ
		
}All_Reg_T;

/*
typedef struct Rcv_Send_Data
{
	u8 can_id;//////����Ǵ������ݵ�id��
	u8 command;///������
	u8 data_addr;///���ݵ�ַ
	u8 data_len;
	u8 data[Communicate_Data_Len];///��������
}Rcv_Send_Data_T;
*/

extern All_Reg_T Reg_Data;

//extern Rcv_Send_Data_T Rcv_Send_Data;
extern u8 Rcv_Current_Count;////�����ڽ������ݵ�ʱ���¼����һ֡�ĵڼ���λ��
extern u8 One_Fram_End_Flag;//һ֡���ݴ��������־
extern u8 Fram_Over_Delay_Time_Flag;////ʱ�������־

extern u8 Fram_Over_Delay_Time;///һ֡������ʱʱ��


extern u8 Rcv_Data[Communicate_Data_Len];////�������ݻ�����
extern u8 Rcv_Data_Len;////�������ݳ���

extern u8 Send_Data[Communicate_Data_Len];///���ͻ�����
extern u8 Send_Data_Len;///���ͳ���

extern u16 Save_All_Data_Time;////��ʱ�������мĴ���ֵ


void save_all_data(void);////�����е�����д����Ӧ��flashλ��
void init_all_data(u8 num);///��ʼ��������ʱ����num 0��ʼ����1������ʼ��
void uart_init(void);////���ڳ�ʼ������
int crc_check( unsigned char *buf,int lenth);
void uart_rcv_handler2(void);
void uart_comm(void);
void Send_Handle(void);

#endif
