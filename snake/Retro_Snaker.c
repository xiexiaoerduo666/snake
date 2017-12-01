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

/*̰���߳�ʼ��*/
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
	count=3;					//������Ϊ3
	table[count]=0;
	table[count+1]=0;
	IT0=1;						//�͵�ƽ����
	ET0=1;						//��ʱ��0�ж�ʹ��
	ET1=1;						//��ʱ��1�ж�ʹ��
	TMOD=0x11;					//��ʱ��0ģʽ1����ʱ��1ģʽ1
	TH0=(65535-50000)/256;		//��ʱ��0�趨��ֵ
	TL0=(65535-50000)%256;
	TH1=(65535-10000)/256;		//��ʱ��1�趨��ֵ
	TL1=(65535-10000)%256;
	Direction=0;						//����
	TR0=0;						//��ʱ��0������
	TR1=1;						//��ʱ��1����
}


/*̰�����жϺ���*/
void snake_isr1()
{
	TH1=(65535-10000)/256;		//��ʱ��1�趨��ֵ
	TL1=(65535-10000)%256;

	tt++;			  //��ȡ�����������
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

/*����ɨ��*/
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
			Flag_Dot=1;			//������Ϸ
			TR0 = 1;			//��ʼ�ƶ�
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
			Flag_Dot=1;			//������Ϸ
			TR0 = 1;			//��ʼ�ƶ�
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

void Shift()       //̰�����ƶ�����
{
	int k;
	if ( Flag_Draw == 1 )
	{
		for(k=0;k<count-1;k++)				//��������
		{
			table[k]=table[k+1];
		}
	}
	switch (Direction)
	{
		case 0:							//����
			if (table[count-1]/16<15)				//��ͷû�г����ұ߿�
				table[count-1]=table[count-1]+16;	//����
			else
				Flag_Over=1;							//��ͷ�����߿���Ϸ����
			break;
		case 1:							//����
			if (table[count-1]%16<15)
				table[count-1]=table[count-1]+1;
			else
				Flag_Over=1;
			break;
		case 2:							//����
			if (table[count-1]/16>0)
				table[count-1]=table[count-1]-16;
			else
				Flag_Over=1;
			break;
		case 3:							//����
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
		for (k=0;k<count-1;k++)							//�ж��Ƿ���ͷ��ײ������
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
	else if (table[count-1]==table[count])		//��ͷ ���� ʳ��
	{
		if (count<30)							
		{
			count++;							//������+1
			Flag_Draw=0;
		}
		Flag_Dot=1;
		table[count]=0xff;						//�µ�ʳ���������û���ǰ������ʾ
	}
	else
	{
		Flag_Draw=1;
	}		
}
void Random_Dot()	    //ȡ�����
{
	int k;
	Flag_Dot=0;
	do
	{
		flag3=1;
		u=rand()%256;				//ȡ��0-255�е�����һ��ֵ
		u=u/16*10+u%16;				//ȡ���漴����кŷŸ�4λ���кŷŵ�4λ
		for (k=0;k<count;k++)		//��ѯ�������漴���Ƿ��غ�
		{
			if (u==table[k])		//�������غϣ������漴
			{
				flag3=0;
				break;
			}
		}
	}
	while(!flag3);					//����flag3 == 1����ѭ�������Ϊ0�������漴��
	table[count]=u;					//ȡ���漴���ֵ����table[count]��
	snake_speed+=1500;
}


//************************************************************
//��ʾ����
//*************************************************************
void Snake_Display()					
{
	unsigned char i,j;
	unsigned int temp = 0x7fff;
	unsigned char x,y;
	while(!ir_flag);
	

	for( j = 0 ; j <= count ; j++ )
	{		LEDARRAY_G = 1;			//��������ʱ��ر���ʾ���ȸ��������ݣ���138��ʾ�С���ֹ��Ӱ��

		y=table[j]/16;				//��4λ����ֵ��  ��ֵ��0 = ѡͨ  1 = ��ѡ
		x=table[j]%16;				//��4λ����ֵ��  ��ֵ��1 = ѡͨ  0 = ��ѡ


		temp = 0x7fff;		
		for(i = 0 ; i < y ; i++)
		{
			temp = (temp>>1)|0x8000;
		} 

		Display_Buffer[1] = temp&0x00ff;
		Display_Buffer[0] = (temp>>8)&0x00ff;
				
		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//��������
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(x);						//ѡ���i��

		LEDARRAY_G = 0;
		
		delay(50);		//��ʱһ��ʱ�䣬��LED��������	
	}			
		
}


