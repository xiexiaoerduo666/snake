#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <reg52.h>
#include <intrins.h>
#include <stdlib.h>

#define Right 0
#define Left 2
#define Up 3
#define Down 1

/*配置IO口*/
sbit LEDARRAY_D = P1^0;
sbit LEDARRAY_C = P1^1;
sbit LEDARRAY_B = P1^2;
sbit LEDARRAY_A = P1^3;
sbit LEDARRAY_G = P1^4;
sbit LEDARRAY_DI = P1^5;
sbit LEDARRAY_CLK = P1^6;
sbit LEDARRAY_LAT = P1^7;

//按键IO配置
sbit KEY_Right=P3^4;			//右
sbit KEY_Down=P3^5;			//下
sbit KEY_Left=P3^3;			//左
sbit KEY_Up=P3^6;			//上

sbit IRIN=P3^2;           //红外接收管脚

 /*对公用的变量声明*/
extern unsigned char Display_Buffer[2];    
extern unsigned char IrValue[6];
extern unsigned char Flag_Shift,Flag_Dot,Flag_Over;
extern unsigned char display_flag;
extern bit ir_flag;
extern unsigned int snake_speed;

/*函数声明*/
void Delay_1ms( unsigned int m);
void delay(int n);
void DelayMs(unsigned int x);   //0.14ms误差 0us
void Scan_Line( unsigned char m);   //扫描一行函数
void Send( unsigned char dat);
void IrInit();	      //红外遥控初始化

/*贪吃蛇函数声明*/
void snake_main();
void snake_isr0();
void snake_isr1();
void ReInit();
void Init();
void Scan_Key();
void Shift();
void Random_Dot();
void Snake_Display();

/*图像移动函数声明*/
void Picture_Display(unsigned char dat[][32]);   //图片显示
void Picture_isr0_init();    //图片中断初始化
void Picture_isr0();        //中断函数声明
void picture_init();
void picture_main();
void Calc_Shift1(); 	  //从右往左移
void Calc_Shift2();   	//从左往右移
void Calc_Shift3();   	//从右往左移
void Calc_Shift4();   	//从左往右移
void Picture_Clear_Display();   //清空缓冲区
void picture1_main();









#endif