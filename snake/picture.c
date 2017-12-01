#include "include.h"
#include "picture.h"

#define Display_Num_Word 1				//液晶能显示的汉字个数
#define Shift_Speed 5					//移动速度

volatile unsigned char Display_Buffer[2];
unsigned char Display_Swap_Buffer[Display_Num_Word][32]={0};  //显示缓冲区
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
		Display_Word_Count = Shift_Count/16;				//计算当前显示第几个字
		Calc_Shift1(); //右移
					
		Shift_Count++;
	}	
	if(Shift_Count == (Num_Of_Word+1)*16 )	
	{
	   Picture_Clear_Display();
	   Shift_Count = 0;
	   zuo=1;
	}
    if(zuo==1&&Flag_Shift == 1 )				//移动次数
		{	
		    Flag_Shift = 0;
			Display_Word_Count = Shift_Count/16;
//			    Clear_Display();
//				Shift_Count = 0;
			Calc_Shift2();//左移
		    Shift_Count++;	
		   	if(Shift_Count == (Num_Of_Word+1)*16 )				//移动次数
		  {
			Shift_Count = 0;	
			zuo=0;			
		  }		
		}
// 			Clear_Display();
//			Delay(10000);	
//			Calc_Shift2();//左移
//			Shift_Count++;
//			if(Shift_Count == (Num_Of_Word+1)*16 )				//移动次数
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
		Display_Word_Count = Shift_Count/16;				//计算当前显示第几个字
		Calc_Shift3(); //右移
					
		Shift_Count++;
	}	
	if(Shift_Count == (Num_Of_Word+1)*16 )	
	{
	   Picture_Clear_Display();
	   Shift_Count = 0;
	   zuo=1;
	}
    if(zuo==1&&Flag_Shift == 1 )				//移动次数
	{	
	    Flag_Shift = 0;
		Display_Word_Count = Shift_Count/16;
//			    Clear_Display();
//				Shift_Count = 0;
		Calc_Shift4();//左移
	    Shift_Count++;	
	   	if(Shift_Count == (Num_Of_Word+1)*16 )				//移动次数
	  {
		Shift_Count = 0;	
		zuo=0;			
	  }		
	}
// 			Clear_Display();
//			Delay(10000);	
//			Calc_Shift2();//左移
//			Shift_Count++;
//			if(Shift_Count == (Num_Of_Word+1)*16 )				//移动次数
//			{
//				Shift_Count = 0;				
//			}			
}
void Picture_Display(unsigned char dat[][32])   //图片显示					
{
	unsigned char i,j;

	for( i = 0 ; i < 16 ; i++ )
	{
		LEDARRAY_G = 1;			//更新数据时候关闭显示。等更新完数据，打开138显示行。防止重影。

		Display_Buffer[0] = dat[0][i];		
		Display_Buffer[1] = dat[0][i+16];

		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//锁存数据
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(i);						//选择第i行

		LEDARRAY_G = 0;
		
		for( j = 0 ; j < 100 ; j++);		//延时一段时间，让LED亮起来。				
	}	
}



void Picture_isr0_init()
{
	TMOD &= 0xF0;
	TMOD |= 0X08;			//T0, 工作模式1

	TH0 = (65536 - 50000)/256;
	TL0 = (65536 - 50000)%256;

	TR0 = 1;				//开启定时器
	ET0 = 1;			    //开启定时器中断
//	EA = 1;					//开启总中断		
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



void Calc_Shift1()	  //从右往左移
{
	unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)									//前8位	移位
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe; 			//最低位清零	
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;			//最低位置一 	
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
			Shift_Bit = 1;						//把字移出缓冲区
		}

		if( Shift_Bit == 0)														//后8位移位
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe; 		//最低位清零
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;		//最低位置一 			
		}
		
	}
	temp1 = (temp1>>1)&0x7f;
	if(temp1 == 0x00)
	{
		temp1 = 0x80;
	}
}



void Calc_Shift2()	//从左往右移
{
	unsigned char k;

	for(k = 0;k < 16;k++)
	{
		if((Display_Swap_Buffer[0][k]&0x01) == 0)									//前8位	移位
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)&0x7f; 			//最低位清零	
		}
		else
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)|0x80;			//最低位置一 	
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
			Shift_Bit = 1;						//把字移出缓冲区
		}

		if( Shift_Bit == 0)														//后8位移位
		{
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)&0x7f; 		//最低位清零
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)|0x80;		//最低位置一 			
		}
		
	}
	temp2 = (temp2<<1)&0xfe;
	if(temp2 == 0x00)
	{
		temp2 = 0x01;
	}
}
void Calc_Shift3()	//从右往左移
{
unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)									//前8位	移位
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe; 			//最低位清零	
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;			//最低位置一 	
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
			Shift_Bit = 1;						//把字移出缓冲区
		}

		if( Shift_Bit == 0)														//后8位移位
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe; 		//最低位清零
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;		//最低位置一 			
		}
		
	}
	temp1 = (temp1>>1)&0x7f;
	if(temp1 == 0x00)
	{
		temp1 = 0x80;
	}
}
void Calc_Shift4()	  //从右往左移
{
	unsigned char k;

	for(k = 0;k < 16;k++)
	{
		if((Display_Swap_Buffer[0][k]&0x01) == 0)									//前8位	移位
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)&0x7f; 			//最低位清零	
		}
		else
		{
			Display_Swap_Buffer[0][16+k] = (Display_Swap_Buffer[0][16+k] >> 1)|0x80;			//最低位置一 	
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
			Shift_Bit = 1;						//把字移出缓冲区
		}

		if( Shift_Bit == 0)														//后8位移位
		{
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)&0x7f; 		//最低位清零
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][k] = (Display_Swap_Buffer[0][k] >> 1)|0x80;		//最低位置一 			
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
	for(j = 0 ; j < Display_Num_Word; j++)	 //液晶能显示的汉字个数
	{
		for(i = 0 ; i < 32 ;i++)
		{
			Display_Swap_Buffer[j][i] = 0xff;				//0=显示  1=不显示 	
		}
	}
}
