#ifndef __JLX_LCD_V2_0_H__
#define __JLX_LCD_V2_0_H__

#include <intrins.h>
#include "STC15F2K60S2.H"


sbit SDAT = P2 ^ 0;//����
sbit SCLK = P2 ^ 1;//ʱ��
sbit CD = P2 ^ 2;//�Ĵ���ѡ�񣬸ߵ�ƽ�������ݣ��͵�ƽ����ָ��
sbit RS = P2 ^ 3;//��λ���͵�ƽ��λ����λ��ɺ󣬻ص��ߵ�ƽ
sbit CS = P2 ^ 4;//Ƭѡ���͵�ƽƬѡ
sbit LCD_LED = P2 ^ 5;//


void timer(unsigned int t);
void wlc(unsigned char dat);//д��Һ����ָ�write lcd command
void wld(unsigned char dat);//д��Һ�������ݣ�write lcd data
void InitLCD();//��ʼ��Һ����
void CleanLCD();//����
void LCDAddress(unsigned char page, unsigned char column);//����д���ַ��ҳ���о���0��ʼ
void DrawPoint(unsigned char line, unsigned char column);//��ָ�����л�һ���㣬�Ḳ��ԭ��������
void display0805(unsigned char page, unsigned char column, unsigned char *p);//��ָ��ҳ��ָ����д��8*5��ͼ������
void display1608(unsigned char page, unsigned char column, unsigned char *p);//��ָ��ҳ��ָ����д��16*8��ͼ������
void display1616(unsigned char page, unsigned char column, unsigned char *p);//��ָ��ҳ��ָ����д��16*16��ͼ������
void display3232(unsigned char page, unsigned char column, unsigned char *p);//��ָ��ҳ��ָ����д��32*32��ͼ������
void display6464(unsigned char column, unsigned char *p);//��ָ����д��64*64��ͼ������
void display19264(unsigned char *p);//��������Ĺ��ܲ�����
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ0805��Ӣ���ַ�����֧���Զ����С���֧��%d�����ģ���Ĺ��ܺ�printf()һ����
void DisplayString0805(unsigned char page, unsigned char column, unsigned char *p);
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ0805�����֣��ֲ�DisplayString()���ܶ�̬��ʾ��ȱ�ݡ�
//ע�⣺�����������޷������ͣ���֧���Զ����У�ʹ��ʱ��ʾ�����ֲ�Ҫ�����߽磡��
void DisplayNumber0805(unsigned char page, unsigned char column, unsigned int a);
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ1608��Ӣ���ַ�������֧���Զ����У���֧���Զ����У���֧���Զ����У�
void DisplayString1608(unsigned char page, unsigned char column, unsigned char *p);
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ1608�����֣�ϸ������ͬDisplayNumber
void DisplayNumber1608(unsigned char page, unsigned char column, unsigned int a);

//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ1616���ַ���
void DisplayString1616(unsigned char page, unsigned char column,unsigned char *p);

#endif
