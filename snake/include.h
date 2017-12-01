#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <reg52.h>
#include <intrins.h>
#include <stdlib.h>

#define Right 0
#define Left 2
#define Up 3
#define Down 1

/*����IO��*/
sbit LEDARRAY_D = P1^0;
sbit LEDARRAY_C = P1^1;
sbit LEDARRAY_B = P1^2;
sbit LEDARRAY_A = P1^3;
sbit LEDARRAY_G = P1^4;
sbit LEDARRAY_DI = P1^5;
sbit LEDARRAY_CLK = P1^6;
sbit LEDARRAY_LAT = P1^7;

//����IO����
sbit KEY_Right=P3^4;			//��
sbit KEY_Down=P3^5;			//��
sbit KEY_Left=P3^3;			//��
sbit KEY_Up=P3^6;			//��

sbit IRIN=P3^2;           //������չܽ�

 /*�Թ��õı�������*/
extern unsigned char Display_Buffer[2];    
extern unsigned char IrValue[6];
extern unsigned char Flag_Shift,Flag_Dot,Flag_Over;
extern unsigned char display_flag;
extern bit ir_flag;
extern unsigned int snake_speed;

/*��������*/
void Delay_1ms( unsigned int m);
void delay(int n);
void DelayMs(unsigned int x);   //0.14ms��� 0us
void Scan_Line( unsigned char m);   //ɨ��һ�к���
void Send( unsigned char dat);
void IrInit();	      //����ң�س�ʼ��

/*̰���ߺ�������*/
void snake_main();
void snake_isr0();
void snake_isr1();
void ReInit();
void Init();
void Scan_Key();
void Shift();
void Random_Dot();
void Snake_Display();

/*ͼ���ƶ���������*/
void Picture_Display(unsigned char dat[][32]);   //ͼƬ��ʾ
void Picture_isr0_init();    //ͼƬ�жϳ�ʼ��
void Picture_isr0();        //�жϺ�������
void picture_init();
void picture_main();
void Calc_Shift1(); 	  //����������
void Calc_Shift2();   	//����������
void Calc_Shift3();   	//����������
void Calc_Shift4();   	//����������
void Picture_Clear_Display();   //��ջ�����
void picture1_main();









#endif