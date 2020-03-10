#include "bsp_flash.h"


u8 default_data[Default_Data_Len] = {8, 2, 25, 25, 25, 0x3c};///Ĭ�Ϸ����Զ���ѡ�����䣬�����¶�25������25�������¶�25
//u8 flashdata_init[7] = {0x00,0x02,0x16,0x15, 0x14, 0x3c,27};
u8 flashdata_init[7] = {0};

void flash_init(void)////flash ��ʼ������
{
	��������
}


#if 0
void __eeprom_write_many(unsigned short addr_eep,unsigned short size,unsigned short addr_ram) 
{ 
   
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);   
  FLASH_Unlock(FLASH_MEMTYPE_DATA); 
   
  for(unsigned short i=0;i<size;i++) 
  { 
     FLASH_ProgramByte(addr_eep+i,   *((unsigned char *)(addr_ram)+i));  
     FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);  
  } 
   
  FLASH_Lock(FLASH_MEMTYPE_DATA);   
} 
#endif

/***********************
**����:����������Flashд����,���ֽ�д������Ϊlen
**����:add ��ַ    dataҪд������ʼ��ַ��lenҪд�����ݳ���
**����:none
***********************/
void write_flash(u32 addr, u8 *data, u8 len)
{
	 unsigned char i = 0;
	
	//FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	for(i = 0; i < len; i++)
	{
		//FLASH_EraseByte(addr+i);
		//FLASH_WaitForLastOperation(FLASH_MemType_Program);
		
		FLASH_ProgramByte((u32)(addr+i), data[i]);  
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
	}			
	FLASH_Lock(FLASH_MEMTYPE_DATA);
}



/***********************
**����:��Flash���ֽ�һ��һ���Ķ���������
**����:addrҪ�����ݵ�ַ��Ҫ�����ݵ�������Ҫ�����ݵĳ��ȣ�
**����:none
***********************/
void read_flash(u32 addr, u8 *data, u8 len)
{
	u8 i = 0;
	//FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	for(i = 0; i < len; i++)
	{
		data[i] = FLASH_ReadByte(addr+i);
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
	}
								
	FLASH_Lock(FLASH_MEMTYPE_DATA);
	
}

void flash_default_init(void)////��ʼ��ʱ�洢flashĬ��ֵ
{
	write_flash((u32)(FLASH_ADDR_BASE+Default_Set_Offset), default_data, (u8)Default_Data_Len);
}

void flash_set_init(void)
{
#if 1
    u8 read_temp[7] = {0};
    read_flash((u32)(FLASH_ADDR_BASE+Set_Data_Offset), flashdata_init, 7);
    if((flashdata_init[0] == 0xff)||(flashdata_init[0] == 0x00))
    {
        flashdata_init[0] = 0x00;
    }
    if((flashdata_init[1] == 0xff)||(flashdata_init[1] == 0x00))
    {
        flashdata_init[1] = 0x00;
    }
     if((flashdata_init[2] == 0xff)||(flashdata_init[2] == 0x00))
    {
        flashdata_init[2] = 0x16;
    }
    if((flashdata_init[3] == 0xff)||(flashdata_init[3] == 0x00))
    {
        flashdata_init[3] = 0x15;
    }
    if((flashdata_init[4] == 0xff)||(flashdata_init[4] == 0x00))
    {
        flashdata_init[4] = 0x14;
    }
    if((flashdata_init[5] == 0)||(flashdata_init[5] == 0xFF))
    {
        flashdata_init[5] = 0x3C;
    }
    if((flashdata_init[6] == 0xff)||(flashdata_init[6] == 0x00))
    {
        flashdata_init[6] = 27;
    }
    
#endif
    
    write_flash((u32)(FLASH_ADDR_BASE+Set_Data_Offset), flashdata_init, 7);
}
