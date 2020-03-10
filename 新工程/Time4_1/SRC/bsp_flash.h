#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H
#include "myinclude.h"

#define FLASH_ADDR_BASE 0x4000


#define Set_Data_Offset 0
#define Default_Set_Offset 10///Ĭ�ϵ����ô洢λ��


#define Default_Data_Len 7


void write_flash(u32 addr, u8 *data, u8 len);


void read_flash(u32 addr, u8 *data, u8 len);

void flash_default_init(void);////��ʼ��ʱ�洢flashĬ��ֵ

void flash_set_init(void);

#endif
