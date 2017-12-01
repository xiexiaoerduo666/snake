#include <reg52.h>
#include "include.h"

volatile unsigned char display_flag=0;

void main()
{

	IrInit();
	while(1)
	{
		switch(display_flag)
		{
			case 0:
			{	
				picture_init();
				while(display_flag==0)
				{  
					picture_main();
				}
			break;
			}
			case 1:
			{
				Init();	
				Delay_1ms(100);
				while(display_flag==1)
				{  
					snake_main();
				}
			break;
			case 2:
			{	
				picture_init();
				while(display_flag==2)
				{  
					picture1_main();
				}
			break;
			}
			}
		}
	}
}

/*�������ݸ�595*/
void Send( unsigned char dat)
{
	unsigned char i;
	char j = 0;
	LEDARRAY_CLK = 0;
	_nop_();	
	LEDARRAY_LAT = 0;
	_nop_();

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			LEDARRAY_DI = 1;	
		}
		else
		{
			LEDARRAY_DI = 0;
		}


		LEDARRAY_CLK = 1;				//�����ط�������
			_nop_();
		LEDARRAY_CLK = 0;
			_nop_();

		
		dat >>= 1;
			
	}			
}

/*138��ʾ����,ɨ��һ�У�����ˢ��*/
void Scan_Line( unsigned char m)
{	
	switch(m)
	{
		case 0:			
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0; 					
			break;
		case 1:					
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 2:					
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 3:					
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 4:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 5:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 6:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 7:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 8:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 9:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;	
		case 10:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 11:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 12:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 13:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 14:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 15:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		default : break;	
	}
}

void IrInit()	      //����ң�س�ʼ��
{
	IT0=1;//�½��ش���
	EX0=1;//���ж�0����
	EA=1;	//�����ж�

	IRIN=1;//��ʼ���˿�
}
//��ʱ����
void Delay_1ms( unsigned int m)
{
	unsigned int j;
	unsigned int i;
	for(i = 0 ; i < m ; i++)
	{
		for(j = 0 ; j < 123 ; j++);
	}
}


void delay(int n)
{
	while(n--);
}


void DelayMs(unsigned int x)   //0.14ms��� 0us
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++)
		{
		}
	}
}

void snake_main()	     //̰����������
{
	if (Flag_Dot == 1)			//������������
		Random_Dot();			//�漴��ʾһ����
	if (Flag_Shift == 1)		
		Shift();				//̰ʳ���ƶ�

	Scan_Key();
	Snake_Display();	
}


