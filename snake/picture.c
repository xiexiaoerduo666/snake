#include "include.h"
#include "picture.h"

#define Display_Num_Word 1				//Һ������ʾ�ĺ��ָ���
#define Shift_Speed 5					//�ƶ��ٶ�

volatile unsigned char Display_Buffer[2];
unsigned char Display_Swap_Buffer[Display_Num_Word][32]={0};  //��ʾ������
volatile bit zuo=0;
bit Shift_Bit = 0;
unsigned char Timer0_Count = 0;
unsigned char temp1 = 0x80;
unsigned char temp2 = 0x01;
unsigned char Shift_Count = 0;
unsigned char Display_Word_Count = 0;

void picture_init()
{
	Flag_Shift=0;
	Shift_Bit=0;
	temp1 = 0x80;
	temp2 = 0x01;
	Timer0_Count=0;
	Shift_Count =0;
	Display_Word_Count=0;
	Picture_isr0_init();
	Picture_Clear_Display();
}

									  		
void picture_main()
{
    Picture_Display(Display_Swap_Buffer);
	if(Flag_Shift == 1&&zuo==0)
	{
		Flag_Shift = 0;
		Display_Word_Count = Shift_Count/16;				//���㵱ǰ��ʾ�ڼ�����
		Calc_Shift1(); //����
					
		Shift_Count++;
	}	
	if(Shift_Count == (Num_Of_Word+1)*16 )	
	{
	   Picture_Clear_Display();
	   Shift_Count = 0;
	   zuo=1;
	}
    if(zuo==1&&Flag_Shift == 1 )				//�ƶ�����
		{	
		    Flag_Shift = 0;
			Display_Word_Count = Shift_Count/16;
//			    Clear_Display();
//				Shift_Count = 0;
			Calc_Shift2();//����
		    Shift_Count++;	
		   	if(Shift_Count == (Num_Of_Word+1)*16 )				//�ƶ�����
		  {
			Shift_Count = 0;	
			zuo=0;			
		  }		
		}
// 			Clear_Display();
//			Delay(10000);	
//			Calc_Shift2();//����
//			Shift_Count++;
//			if(Shift_Count == (Num_Of_Word+1)*16 )				//�ƶ�����
//			{
//				Shift_Count = 0;				
//			}			
}
void picture1_main()
{ 
    Picture_Display(Display_Swap_Buffer);
	if(Flag_Shift == 1&&zuo==0)
	{
		Flag_Shift = 0;
		Display_Word_Count = Shift_Count/16;				//���㵱ǰ��ʾ�ڼ�����
		Calc_Shift3(); //����
					
		Shift_Count++;
	}	
	if(Shift_Count == (Num_Of_Word+1)*16 )	
	{
	   Picture_Clear_Display();
	   Shift_Count = 0;
	   zuo=1;
	}
    if(zuo==1&&Flag_Shift == 1 )				//�ƶ�����
	{	
	    Flag_Shift = 0;
		Display_Word_Count = Shift_Count/16;
//			    Clear_Display();
//				Shift_Count = 0;
		Calc_Shift4();//����
	    Shift_Count++;	
	   	if(Shift_Count == (Num_Of_Word+1)*16 )				//�ƶ�����
	  {
		Shift_Count = 0;	
		zuo=0;			
	  }		
	}
// 			Clear_Display();
//			Delay(10000);	
//			Calc_Shift2();//����
//			Shift_Count++;
//			if(Shift_Count == (Num_Of_Word+1)*16 )				//�ƶ�����
//			{
//				Shift_Count = 0;				
//			}			
}
void Picture_Display(unsigned char dat[][32])   //ͼƬ��ʾ					
{
	unsigned char i,j;

	for( i = 0 ; i < 16 ; i++ )
	{
		LEDARRAY_G = 1;			//��������ʱ��ر���ʾ���ȸ��������ݣ���138��ʾ�С���ֹ��Ӱ��

		Display_Buffer[0] = dat[0][i];		
		Display_Buffer[1] = dat[0][i+16];

		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//��������
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(i);						//ѡ���i��

		LEDARRAY_G = 0;
		
		for( j = 0 ; j < 100 ; j++);		//��ʱһ��ʱ�䣬��LED��������				
	}	
}



void Picture_isr0_init()
{
	TMOD &= 0xF0;
	TMOD |= 0X08;			//T0, ����ģʽ1

	TH0 = (65536 - 50000)/256;
	TL0 = (65536 - 50000)%256;

	TR0 = 1;				//������ʱ��
	ET0 = 1;			    //������ʱ���ж�
//	EA = 1;					//�������ж�		
}
void Picture_isr0()
{
	TH0 = (65536 - 50000)/256;
	TL0 = (65536 - 50000)%256;
	
	TF0 = 0;
	Timer0_Count++;
	if(Timer0_Count >= Shift_Speed)
	{
		Timer0_Count = 0;
		Flag_Shift = 1;
	}

}



void Calc_Shift1()	  //����������
{
	unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)									//ǰ8λ	��λ
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe; 			//���λ����	
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;			//���λ��һ 	
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word1[Display_Word_Count][i]&temp1;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word1[Display_Word_Count][16+i]&temp1;
		}
		else
		{
			Shift_Bit = 1;						//�����Ƴ�������
		}

		if( Shift_Bit == 0)														//��8λ��λ
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe; 		//���λ����
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;		//���λ��һ 			
		}
		
	}
	temp1 = (temp1>>1)&0x7f;
	if(temp1 == 0x00)
	{
		temp1 = 0x80;
	}
}



void Calc_Shift2()	//����������
{
	unsigned char k;

	for(k = 0;k < 16;k++)
	{
		if((Display_Swap_Buffer[0][k]&0x01) == 0)									//ǰ8λ	��λ
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)&0x7f; 			//���λ����	
		}
		else
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)|0x80;			//���λ��һ 	
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word2[Display_Word_Count][16+k]&temp2;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word2[Display_Word_Count][k]&temp2;
		}
		else
		{
			Shift_Bit = 1;						//�����Ƴ�������
		}

		if( Shift_Bit == 0)														//��8λ��λ
		{
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)&0x7f; 		//���λ����
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)|0x80;		//���λ��һ 			
		}
		
	}
	temp2 = (temp2<<1)&0xfe;
	if(temp2 == 0x00)
	{
		temp2 = 0x01;
	}
}
void Calc_Shift3()	//����������
{
unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)									//ǰ8λ	��λ
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe; 			//���λ����	
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;			//���λ��һ 	
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word3[Display_Word_Count][i]&temp1;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word3[Display_Word_Count][16+i]&temp1;
		}
		else
		{
			Shift_Bit = 1;						//�����Ƴ�������
		}

		if( Shift_Bit == 0)														//��8λ��λ
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe; 		//���λ����
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;		//���λ��һ 			
		}
		
	}
	temp1 = (temp1>>1)&0x7f;
	if(temp1 == 0x00)
	{
		temp1 = 0x80;
	}
}
void Calc_Shift4()	  //����������
{
	unsigned char k;

	for(k = 0;k < 16;k++)
	{
		if((Display_Swap_Buffer[0][k]&0x01) == 0)									//ǰ8λ	��λ
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)&0x7f; 			//���λ����	
		}
		else
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)|0x80;			//���λ��һ 	
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word4[Display_Word_Count][16+k]&temp2;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word4[Display_Word_Count][k]&temp2;
		}
		else
		{
			Shift_Bit = 1;						//�����Ƴ�������
		}

		if( Shift_Bit == 0)														//��8λ��λ
		{
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)&0x7f; 		//���λ����
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)|0x80;		//���λ��һ 			
		}
		
	}
	temp2 = (temp2<<1)&0xfe;
	if(temp2 == 0x00)
	{
		temp2 = 0x01;
	}
}


void Picture_Clear_Display()
{
	unsigned char i,j;
	for(j = 0 ; j < Display_Num_Word; j++)	 //Һ������ʾ�ĺ��ָ���
	{
		for(i = 0 ; i < 32 ;i++)
		{
			Display_Swap_Buffer[j][i] = 0xff;				//0=��ʾ  1=����ʾ 	
		}
	}
}
