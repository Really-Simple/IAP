#ifndef _IAP_H
#define _IAP_H

#include "stm32f10x.h"

typedef void (*iapfun)(void);//����һ���������͵Ĳ���������ָ�룬����ֵ���βζ�Ϊ��

#define FLASH_APP1_ADDR		0x08010000 

void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize);
void iap_load_app(u32 appxaddr);

#endif

