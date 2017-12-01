/* 中断函数查询 */
#include "include.h"
#include <reg52.h>
unsigned char Time;
volatile unsigned char IrValue[6];

void ReadIr() interrupt 0		//接收到的信号与发送信号正好相反[使用外部中断0]
{
	unsigned char j,k;
	unsigned int err;
	ir_flag=1;
	Time=0;					 
	DelayMs(70);

	if(IRIN==0)		//确认是否真的接收到正确的信号【是否有9毫秒的低电平】
	{	 	
		err=1000;				//1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真时循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/	
		while((IRIN==0)&&(err>0))//1.产生下降沿，进入外部中断0的中断函数，延时一下之后检测IO口是否还是低电平，是就等待前面9ms的低电平过去  		
		{			
			DelayMs(1);
			err--;
		} 
		if(IRIN==1)			//如果正确等到9ms低电平
		{
			err=500;  //5ms
			while((IRIN==1)&&(err>0))		 //2.等待完9毫秒的低电平过去，再等待4.5ms的起始高电平过去
			{
				DelayMs(1);
				err--;
			}
			for(k=0;k<4;k++)		//3.开始接收传递的4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{
					err=60;	//600us	
					while((IRIN==0)&&(err>0))//{1}等待信号前面的560us低电平过去
					{
						DelayMs(1);
						err--;
					}
					err=500;//5ms
					while((IRIN==1)&&(err>0))	 //计算高电平的时间长度。{2}检测高电平的持续时间{565us---1.69ms内是高电平}
					{
						DelayMs(1);//0.14ms
						Time++;//每次加上0.14
						err--;
						if(Time>30)// DelayMs(1);每循环一次,time加一,这里判断Time有没超过30次,即时间有没超过30*0.14ms
						{
							EX0=1;//外部中断允许位打开
							ir_flag=0;
							return;// 退出一个循环,这里是while((IRIN==1)&&(err>0)) 
						}
					}
					IrValue[k]>>=1;	 //k表示第几组数据 ，低位首先发送，然后依次发送另三组数据	  不用赋初值,因为要用的是通过移位后得到的值
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;//1000 0000 通过移位后进行或运算( | ),0x80 就是2进制 1000 000 , 或了之后第八位为1,其他7位保留来的数值
					}
					Time=0;		//用完时间要重新赋值							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])//4.检测接收到的数据与数据的反码进行比较[第三个为数据码，第四个为数据反码]
		{
			ir_flag=0;
			return;
		}
		ir_flag=0;
	}			
}


//**********************************************************
//定时器中断1
//**********************************************************
void time1() interrupt 3
{
	snake_isr1();  //贪吃蛇中断函数
}
//**********************************************************
//定时器0中断
//**********************************************************
void time0() interrupt 1
{
   	switch(display_flag)
	{
		case 0:
		{
			Picture_isr0();	
			if(IrValue[2]==0x1c)
				display_flag=1;
			else
				IrValue[2]=0;	
		break;
		}
		case 1:
		{
			snake_isr0();  //贪吃蛇中断函数
		break;
		case 2:
		{
			Picture_isr0();	
		  	if(IrValue[2]==0x1c)
			{
				display_flag=1;
				snake_speed=0;
			}
			else
				IrValue[2]=0;
		break;
		}
		}
	}
}
