#include "iap.h"
#include "stmflash.h"
#include <stdio.h>

__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE
//appsize:Ӧ�ó���� С(�ֽ�)
iapfun jump2app;
u16 iapbuf[1024];

void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)//�Ѵ��ڽ��յ��Ķ������ļ�д��FLASH
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];
		dfu+=2;//ƫ�������ֽ�
		iapbuf[i++]=temp;//�ѽ��յ��Ķ������ļ���ÿ����һ�飬���´���iapbuf��
		if(i==1024)//�������
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);
			fwaddr+=2048;//ƫ��2048  16=2*8.����Ҫ����2
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ������1024��
}

//��ת��Ӧ�ó���
//appxaddr:�û�������ʼ��ַ
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)//���ջ����ַ�Ƿ�Ϸ�
	{
		jump2app=(iapfun)*(vu32*)(appxaddr+4);
		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
		MSR_MSP(*(vu32*)appxaddr);
		//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();
	}
}
