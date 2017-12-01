#include "include.h"

unsigned char count = 3;
unsigned char Direction;
unsigned char T;
int tt;
volatile unsigned char Flag_Shift,Flag_Dot,Flag_Over=0;
unsigned char flag3,Flag_Draw;
unsigned char u;
volatile unsigned int snake_speed=0;
unsigned char table[31]={0x00,0x10,0x20};
volatile bit ir_flag=0;

/*贪吃蛇初始化*/
void Init()
{
	Flag_Draw=1;
	Flag_Dot=2;
	tt=0;
	T=0;
	Flag_Shift=0; 
	if(Flag_Over==1)
	{
	table[0]=0x00;
	table[1]=0x10;
	table[2]=0x20;
	}
	Flag_Over=0;
	snake_speed=0;
	count=3;					//蛇身长度为3
	table[count]=0;
	table[count+1]=0;
	IT0=1;						//低电平触发
	ET0=1;						//定时器0中断使能
	ET1=1;						//定时器1中断使能
	TMOD=0x11;					//定时器0模式1，定时器1模式1
	TH0=(65535-50000)/256;		//定时器0设定初值
	TL0=(65535-50000)%256;
	TH1=(65535-10000)/256;		//定时器1设定初值
	TL1=(65535-10000)%256;
	Direction=0;						//方向
	TR0=0;						//定时器0不开启
	TR1=1;						//定时器1开启
}


/*贪吃蛇中断函数*/
void snake_isr1()
{
	TH1=(65535-10000)/256;		//定时器1设定初值
	TL1=(65535-10000)%256;

	tt++;			  //获取随机数新种子
	srand(tt);
	if 	(tt>=2157)
	tt=0;
}

void snake_isr0()
{
	TH0=(65535-50000+snake_speed)/256;
	TL0=(65535-50000+snake_speed)%256;
	T++;
	
	if (T>=10)
	{
		T=0;
		Flag_Shift=1;
		
	}
}

/*按键扫描*/
void Scan_Key()
{
     switch (IrValue[2])
	{
	  case 0x18:
	  {
		if(Direction != Down)
		Direction = Up;
		IrValue[2]=0;
			
		break;
	  }
	  case 0x5a:
	  {
		if (Flag_Dot==2)
		{
			Flag_Dot=1;			//启动游戏
			TR0 = 1;			//开始移动
		}
		if(Direction != Left)
			Direction = Right;
		IrValue[2]=0;
			
	    break;
	  }
	  case 0x52:
	  {
		if(Direction != Up)
		Direction = Down;
		IrValue[2]=0;	
		break;	
	  }
	  case 0x08:
	  {
		if(Direction != Right)
		Direction = Left;
		IrValue[2]=0;
		break;
	  }
	 
	  default: break;

	}


	if(KEY_Right == 0)
	{
		if (Flag_Dot==2)
		{
			Flag_Dot=1;			//启动游戏
			TR0 = 1;			//开始移动
		}
		if(Direction != Left)
			Direction = Right;
			
	}

	if(KEY_Down == 0)
	{
		if(Direction != Up)
		Direction = Down;	
	}

	if(KEY_Left == 0)
	{
		if(Direction != Right)
		Direction = Left;
	
	}

	if(KEY_Up == 0)
	{
		if(Direction != Down)
		Direction = Up;	
	}
}

void Shift()       //贪吃蛇移动函数
{
	int k;
	if ( Flag_Draw == 1 )
	{
		for(k=0;k<count-1;k++)				//绘制蛇身
		{
			table[k]=table[k+1];
		}
	}
	switch (Direction)
	{
		case 0:							//向右
			if (table[count-1]/16<15)				//蛇头没有超过右边框
				table[count-1]=table[count-1]+16;	//右移
			else
				Flag_Over=1;							//蛇头超过边框，游戏结束
			break;
		case 1:							//向下
			if (table[count-1]%16<15)
				table[count-1]=table[count-1]+1;
			else
				Flag_Over=1;
			break;
		case 2:							//向左
			if (table[count-1]/16>0)
				table[count-1]=table[count-1]-16;
			else
				Flag_Over=1;
			break;
		case 3:							//向上
			if (table[count-1]%16>0)
				table[count-1]=table[count-1]-1;
			else
				Flag_Over=1;
			break;
		default:
			break;
	}
	Flag_Shift=0;
	if (Flag_Over == 0)
	{
		for (k=0;k<count-1;k++)							//判断是否蛇头碰撞到蛇身
		{
			if (table[count-1]==table[k])
			{
				Flag_Over=1;
				break;
			}
		}
	}
	if (Flag_Over == 1)
	{ 
		display_flag=2;
	}
	else if (table[count-1]==table[count])		//蛇头 碰到 食物
	{
		if (count<30)							
		{
			count++;							//蛇身长度+1
			Flag_Draw=0;
		}
		Flag_Dot=1;
		table[count]=0xff;						//新的食物的坐标在没随机前，不显示
	}
	else
	{
		Flag_Draw=1;
	}		
}
void Random_Dot()	    //取随机点
{
	int k;
	Flag_Dot=0;
	do
	{
		flag3=1;
		u=rand()%256;				//取出0-255中的任意一个值
		u=u/16*10+u%16;				//取出随即点的行号放高4位，列号放低4位
		for (k=0;k<count;k++)		//查询射身与随即点是否重合
		{
			if (u==table[k])		//假如有重合，重新随即
			{
				flag3=0;
				break;
			}
		}
	}
	while(!flag3);					//假如flag3 == 1跳出循环，如果为0则重新随即点
	table[count]=u;					//取出随即点的值放在table[count]中
	snake_speed+=1500;
}


//************************************************************
//显示函数
//*************************************************************
void Snake_Display()					
{
	unsigned char i,j;
	unsigned int temp = 0x7fff;
	unsigned char x,y;
	while(!ir_flag);
	

	for( j = 0 ; j <= count ; j++ )
	{		LEDARRAY_G = 1;			//更新数据时候关闭显示。等更新完数据，打开138显示行。防止重影。

		y=table[j]/16;				//高4位放列值，  行值：0 = 选通  1 = 不选
		x=table[j]%16;				//低4位放行值，  列值：1 = 选通  0 = 不选


		temp = 0x7fff;		
		for(i = 0 ; i < y ; i++)
		{
			temp = (temp>>1)|0x8000;
		} 

		Display_Buffer[1] = temp&0x00ff;
		Display_Buffer[0] = (temp>>8)&0x00ff;
				
		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//锁存数据
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(x);						//选择第i行

		LEDARRAY_G = 0;
		
		delay(50);		//延时一段时间，让LED亮起来。	
	}			
		
}


