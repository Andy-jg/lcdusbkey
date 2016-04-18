

// (ST7565P)

#include "STC15F2K60S2.H"
#include <string.h>
#include <stdlib.h>
#include <intrins.h>
#include "lcdtest.h"
#include "key.h"
#include "englishchar.h"
#include "Chinese16_16.h"
#include "ASCII16.H"

#define NOP_() {_nop_();}
//==============���Ŷ���==================


sbit LCD_LED = P2 ^ 5;//
sbit CS     = P2 ^ 4; //
sbit RST    = P2 ^ 3; //
sbit AO     = P2 ^ 2; //
sbit SDA    = P2 ^ 0; //
sbit SCK    = P2 ^ 1; //


sbit KEY_up     = P0 ^ 2;
sbit KEY_down   = P5 ^ 2;
sbit KEY_cancel = P0 ^ 3;
sbit KEY_ok     = P0 ^ 4;


#define KEY_Up      (!KEY_up)
#define KEY_Down    (!KEY_down)
#define KEY_Cancel  (!KEY_cancel)
#define KEY_Ok      (!KEY_ok)



#define PAGE_S  (0xb0)  //��0ҳ��ַ
#define PAGE_E  (0xb8)  //���ҳ��ַ,��8ҳ

#define COLUMN_OFFSET (4)
#define COLUMN_H   (0x10)  //�е�ַ(��λ)
#define COLUMN_L   (0x00)  //�е�ַ(��λ)




void DelayM (uint a)
{
	//��ʱ���� 1MS/��
	uchar i;

	while (--a != 0) {
		for (i = 0; i < 125; i++); //һ�� ; ��ʾ�����,CPU��ת��
	}                      //i ��0�ӵ�125��CPU��žͺ�ʱ1����
}


uchar getkey(void)
{
	if(KEY_Up){
		DelayM(1000);
		while(KEY_Up);
		return KEY_UP;
	}else if(KEY_Down){
		DelayM(1000);
		while(KEY_Down);
		return KEY_DOWN;
	}else if(KEY_Ok){
		DelayM(1000);
		while(KEY_Ok);
		return KEY_OK;
	}else if (KEY_Cancel){
		DelayM(1000);
		while(KEY_Cancel);
		return KEY_ESC;
	}
		else{
		return KEY_NULL;
	}
}


//=============д����=================
void LcdSt7567_WriteCmd (cmd)
{
	uchar i, j;
	CS = 0;       //Ƭѡ��
	NOP_();
	AO = 0;       //ѡ��д����
	NOP_();
	SCK = 0;      //ʱ������
	NOP_();

	for (i = 0; i < 8; i++) {   //д�˴�
		j = cmd;                //�����ݸ�ֵ������ j
		SCK = 0;                //ʱ������
		SDA = cmd & 0x80;       //��Ϊ����ʼ���� 1111 1111 �����ĸ�ʽ ����һ����һ��ֻ��дһ����  �����ε���λ ȡ���λд��
		NOP_();
		SCK = 1;                //������д������
		NOP_();
		cmd = j << 1;           //��������һλ  Ȼ���ٴ�д�� ֱ���˸�����������ȫ��д��
	}

	NOP_();
	CS = 1;                     //Ƭѡ����λ
	NOP_();                    //��ʱһ��
	AO = 1;                     //�ͷŶ�д�˿�
	NOP_();
	SDA = 1;                    //�ͷ����ݶ˿� ����ʹд����ɺ� û������������ ����ʡ��ôһ���ĵ� ����˵ Ҳ���Բ�����������������˿���ͬʱʹ��
}
//=============д����====================
void LcdSt7567_WriteData (dat)
{
	uchar i, j;
	CS = 0;                 //Ƭѡ��
	NOP_();
	AO = 1;                 //ѡ��д���ݣ�����ʾ��
	NOP_();
	SCK = 0;                //ʱ������
	NOP_();

	for (i = 0; i < 8; i++) {
		j = dat;            //������ֵΪ��Ҫ������
		SCK = 0;            //ʱ���õ�λ
		SDA = dat & 0x80;   //�����Ͱ�λ
		NOP_();
		SCK = 1;            //������д����
		NOP_();
		dat = j << 1;       //��������һλ
	}

	NOP_();
	CS = 1;                 //Ƭѡ�ø�
	NOP_();
	AO = 0;                 //��д�õ�
	NOP_();
	AO = 1;                 //�ͷŶ�д�˿�
	NOP_();
	SDA = 1;                //�ͷ����ݶ˿�
}
//==================Һ����ʼ��=============
void LCD_ST7567_INIT (void)
{
#define INITCODELEN 13
	unsigned char code InitCode[INITCODELEN] = {0xae, 0x2f, 0x22, 0x81, 0x3f, 0xa2, 0xa1, 0xc0, 0x10, 0x00, 0xb0, 0xa6, 0xaf};
	unsigned char i;

	P2M1 = 0x00;
	P2M0 = 0x2F;
	LCD_LED = 1;


	RST = 1;                 //Ӳ��λ�˿��ø�
	DelayM (10);              //��ʱһ�µȴ��ϵ��ȶ�
	RST = 0;                 //Ӳ����λ
	DelayM (10);                 //��ʱһ��
	RST = 1;                 //��λ���

	for (i = 0; i < INITCODELEN ; i++) {
		LcdSt7567_WriteCmd (InitCode[i]);
	}

//	LcdSt7567_WriteCmd (0xE2); //�����λ
//	_nop_();
//	LcdSt7567_WriteCmd (0xA2); //ƫѹ���� ��11���� ok
//	LcdSt7567_WriteCmd (0x00); //ƫѹ���� 0: 1/9 bias, 1: 1/7 bias (ST7565P)
//	LcdSt7567_WriteCmd (0xA0); //��ʾ���� ���� ��8���� ok
//	LcdSt7567_WriteCmd (0xC8); //��ʾ���� ���� ��15���� ok
//	LcdSt7567_WriteCmd (0x27); //���������� ��17���� ok
//	LcdSt7567_WriteCmd (0x81); //�Աȶ�           ok  18����
//	LcdSt7567_WriteCmd (0x05); //                ok
//	LcdSt7567_WriteCmd (0xf8); //˫�ֽ����� ��ѹ//19����
//	LcdSt7567_WriteCmd (0x00);
//	_nop_();
//	LcdSt7567_WriteCmd (0x2F); //��Դ��
//	LcdSt7567_WriteCmd (0xA6); //�׵׺��� a7Ϊ���ֺڵ�
//	LcdSt7567_WriteCmd (0xA4); //������ʾ
//	LcdSt7567_WriteCmd (0x40); //��ʾ��ʼλ��
//	LcdSt7567_WriteCmd (0xAF); //����ʾ
}

//===========LCD_��12864��=============
void Lcd12864_ClearScreen (void)
{
	uchar i, j;

	for (i = 0; i < 8; i++) {           //����д8��
		LcdSt7567_WriteCmd (0xB0 + i);  //����д ��ַ
		LcdSt7567_WriteCmd (0x10);      //����д ��ַ ����λ
		LcdSt7567_WriteCmd (0x04);      //����д ��ַ ����λ

		for (j = 0; j < (128 + COLUMN_OFFSET); j++) {     //����д128��
//			LcdSt7567_WriteData (((j/8)%2)? i%2?0x00:0xff:i%2?0xFF:0x00); //д������
			LcdSt7567_WriteData (0x00); //д������
		}
	}

//		cmd 0xb0,0x10,0x04,
//
//		dat 00 128��
//
//		cmd b1 10 04
//		dat 00 128��
//		cmd b2 10 04
//		dat 00 128��
//		cmd b3 10 04
//		dat 00 128��
//		cmd b4  10 04
//		dat 00 128��
//		cmd b5  10 04
//		dat 00 128��
//		......
//		cmd b8  10 04
//		dat 00 128��
}

/****************��X��X�� ����S0����*********************************************/
// Row �����ַ ��Χ 0-3;
// Col �����ַ ��Χ0-15;
// pStr ����;
// c ������ʾ 0���� 1����
void WRITE_STRING816 (uchar Row, uchar Col,  uchar *pStr, uchar c)
{
	uchar page , col, col_h, col_l;
	uchar i, j;
	page = Row * 2 + PAGE_S;
	col = Col * 8 + COLUMN_OFFSET;
	col_h = ((col >> 4)  | COLUMN_H) & 0x1F;
	col_l = ((col & 0x0f) | COLUMN_L) & 0x0F;

	for (j = 0; j < 2; j++) {
		LcdSt7567_WriteCmd (col_h);
		LcdSt7567_WriteCmd (col_l);
		LcdSt7567_WriteCmd (page + j);

		for (i = 0;  i < 8; i ++) {
			if (c) {
				LcdSt7567_WriteData (~ (* (pStr++)));
			}
			else {
				LcdSt7567_WriteData (* (pStr++));
			}
		}
	}
}

/****************��X��X�� ����S0����*********************************************/
// Row �����ַ ��Χ 0-3;
// Col �����ַ ��Χ0-15;
// pStr ����;
// c ������ʾ 0���� 1����
void WRITE_STRING1616 (uchar Row, uchar Col,  uchar *pStr)
{
	uchar page , col, col_h, col_l;
	uchar i, j;
	page = Row * 2 + PAGE_S;
	col = Col * 8 + COLUMN_OFFSET;
	col_h = ((col >> 4)  | COLUMN_H) & 0x1F;
	col_l = ((col & 0x0f) | COLUMN_L) & 0x0F;

	for (j = 0; j < 2; j++) {
		LcdSt7567_WriteCmd (col_h);
		LcdSt7567_WriteCmd (col_l);
		LcdSt7567_WriteCmd (page+ j );

		for (i = 0;  i < 16; i ++) { 
			LcdSt7567_WriteData (* (pStr++));
			if(i==8){
				LcdSt7567_WriteCmd (page + j );
			}
		}
	}
}

unsigned char FindChnGBK(unsigned char *p, unsigned int *GBKpos)
{
	unsigned int i,j;
	j = sizeof(Chinese1616)/sizeof(tagChinese); 
	for(i=0;i<j;i++)
	{ 
		if (!memcmp(&Chinese1616[i].ChineseGBK,p,2)){
			(*GBKpos) = i;
			return 1;
		}
	}
	return 0;	
}


void DisplayString1616(unsigned char page, unsigned char column,unsigned char *p)
{
	unsigned int GBKPos;
	while(*p){
		if (FindChnGBK(p, &GBKPos)){
			WRITE_STRING1616(page,column,&Chinese1616[GBKPos].ChineseMAP);
			column += 2;
		}
		p+=2;
	}
	
	
//	display1616(0,0,&zhongguo[2]);
//	display1616(0,32,&zhongguo[36]);
}

/****************��X��X�� ����S0����*********************************************/
// Row �����ַ ��Χ 0-7;
// Col �����ַ ��Χ0-15;
// pStr ����;
// c ������ʾ 0���� 1����
void WRITE_STRING88 (uchar Row, uchar Col,  uchar *pStr, uchar c)
{
	uchar page , col, col_h, col_l;
	uchar i, j;
	page = Row  + PAGE_S;
	col = Col * 8 + COLUMN_OFFSET;
	col_h = ((col >> 4)  | COLUMN_H) & 0x1F;
	col_l = ((col & 0x0f) | COLUMN_L) & 0x0F;

	for (j = 0; j < 1; j++) {
		LcdSt7567_WriteCmd (col_h);
		LcdSt7567_WriteCmd (col_l);
		LcdSt7567_WriteCmd (page + j);

		for (i = 0;  i < 8; i ++) {
			if (c) {
				LcdSt7567_WriteData (~ (* (pStr++)));
			}
			else {
				LcdSt7567_WriteData (* (pStr++));
			}
		}
	}
}

void WRITE_STRING88_ADDR (uchar Row, uchar Col/*,  uchar *pStr, uchar c*/)
{
	uchar page , col, col_h, col_l;
	uchar/* i,*/ j;
	page = Row  + PAGE_S;
	col = Col * 8 + COLUMN_OFFSET;
	col_h = ((col >> 4)  | COLUMN_H) & 0x1F;
	col_l = ((col & 0x0f) | COLUMN_L) & 0x0F;

//	for (j = 0; j < 1; j++) {
		LcdSt7567_WriteCmd (col_h);
		LcdSt7567_WriteCmd (col_l);
		LcdSt7567_WriteCmd (page + j);

//		for (i = 0;  i < 8; i ++) {
//			if (c) {
//				LcdSt7567_WriteData (~ (* (pStr++)));
//			}
//			else {
//				LcdSt7567_WriteData (* (pStr++));
//			}
//		}
//	}
}



void displayPhoto (uchar *pic)
{
	uchar a, x;

	for (a = 0; a < 8; a++) {
		LcdSt7567_WriteCmd (0xB0 + a);   //����ҳ��ַ
		LcdSt7567_WriteCmd (0x10);     //�����е�ַ������λ��- 0000
		LcdSt7567_WriteCmd (0x00);     //�����е�ַ������λ��- 0000

		for (x = 0;  x < 128; x ++) {
			LcdSt7567_WriteData (*pic++);
		}
	}
}

void DisplayString1608(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i=0;//, j;
    while(*p)//�ж��ַ������Ƿ����
    {
		WRITE_STRING816 (page, column + i++  , &ASCII16[(*p - 0x20)][0],0);
		p++;
    }
}

void main  (void)
{

	LCD_ST7567_INIT();          //Һ����ʼ��
	Lcd12864_ClearScreen();     //Һ������

//	while (1) 
	{
//		if (KEY_Up) {
			DisplayString1608 (1, 4, "bacon05");
			DisplayString1608 (3, 10, "4GOT10");
		
//			DisplayString1616 (1, 2, "СССС����" ); //��ʾs0��������	
		
//			DisplayString1616 (3, 4, "�й���������" ); //��ʾs0��������	
		while(1);
//		}
//		else if (KEY_Down) {
//			WRITE_STRING816 (0, 1, &s0[0][16], 0);
//		}
//		else if (KEY_Ok) {
//			WRITE_STRING816 (0, 2, &s0[1][16], 0);
//		}
//		else if (KEY_Cancel) {
//			WRITE_STRING816 (0, 3, &s0[2][16], 0);
//		}

//		WRITE_STRING816(0,4,&s0[3][16],0);
//		WRITE_STRING816(0,5,&s0[4][16],0);
//		WRITE_STRING816(0,6,&s0[5][16],0);
//		WRITE_STRING816(0,7,&s0[6][16],0);
//		WRITE_STRING816(0,8,&s0[7][16],0);
//		WRITE_STRING816(0,9,&s0[8][16],0);
//		WRITE_STRING816(0,10,&s0[9][16],0);
//		WRITE_STRING816(0,11,&s0[10][16],0);
//		WRITE_STRING816(0,12,&s0[11][16],0);
//		WRITE_STRING816(0,13,&s0[12][16],0);
//		WRITE_STRING816(0,14,&s0[13][16],0);
//		WRITE_STRING816(0,15,&s0[14][16],0);
//
//		WRITE_STRING816(2,0,&s0[15][16],0); //��ʾs0��������
//		WRITE_STRING816(2,1,&s0[16][16],0);
//		WRITE_STRING816(2,2,&s0[17][16],0);
//		WRITE_STRING816(2,3,&s0[18][16],0);
//		WRITE_STRING816(2,4,&s0[19][16],0);
//		WRITE_STRING816(2,5,&s0[20][16],0);
//		WRITE_STRING816(2,6,&s0[21][16],0);
//		WRITE_STRING816(2,7,&s0[22][16],0);
//		WRITE_STRING816(2,8,&s0[23][16],0);
//		WRITE_STRING816(2,9,&s0[24][16],0);
//		WRITE_STRING816(2,10,&s0[25][16],0);
//		WRITE_STRING816(2,11,&s0[26][16],0);
//		WRITE_STRING816(2,12,&s0[27][16],0);
//		WRITE_STRING816(2,13,&s0[0][0],0);
//		WRITE_STRING816(2,14,&s0[0][16],0);
//		WRITE_STRING816(2,15,&s0[1][16],0);
//
//		DelayM(2000);                   //��ʱ
//		Lcd12864_ClearScreen();         //����
//		WRITE_STRING816(1,0,&s0[0][0],0); //��ʾs0��������
//		WRITE_STRING816(1,1,&s0[0][16],0);
//		WRITE_STRING816(1,2,&s0[1][16],0);
//		WRITE_STRING816(1,3,&s0[2][16],0);
//		WRITE_STRING816(1,4,&s0[3][16],0);
//		WRITE_STRING816(1,5,&s0[4][16],0);
//		WRITE_STRING816(1,6,&s0[5][16],0);
//		WRITE_STRING816(1,7,&s0[6][16],0);
//		WRITE_STRING816(1,8,&s0[7][16],0);
//		WRITE_STRING816(1,9,&s0[8][16],0);
//		WRITE_STRING816(1,10,&s0[9][16],0);
//		WRITE_STRING816(1,11,&s0[10][16],0);
//		WRITE_STRING816(1,12,&s0[11][16],0);
//		WRITE_STRING816(1,13,&s0[12][16],0);
//		WRITE_STRING816(1,14,&s0[13][16],0);
//		WRITE_STRING816(1,15,&s0[14][16],0);
//
//		WRITE_STRING816(3,0,&s0[15][16],0); //��ʾs0��������
//		WRITE_STRING816(3,1,&s0[16][16],0);
//		WRITE_STRING816(3,2,&s0[17][16],0);
//		WRITE_STRING816(3,3,&s0[18][16],0);
//		WRITE_STRING816(3,4,&s0[19][16],0);
//		WRITE_STRING816(3,5,&s0[20][16],0);
//		WRITE_STRING816(3,6,&s0[21][16],0);
//		WRITE_STRING816(3,7,&s0[22][16],0);
//		WRITE_STRING816(3,8,&s0[23][16],0);
//		WRITE_STRING816(3,9,&s0[24][16],0);
//		WRITE_STRING816(3,10,&s0[25][16],0);
//		WRITE_STRING816(3,11,&s0[26][16],0);
//		WRITE_STRING816(3,12,&s0[27][16],0);
//		WRITE_STRING816(3,13,&s0[0][0],0);
//		WRITE_STRING816(3,14,&s0[0][16],0);
//		WRITE_STRING816(3,15,&s0[1][16],0);
//		DelayM(2000);                 //��ʱ
//		Lcd12864_ClearScreen();       //����
//		DelayM (2000);                //��ʱ
//		Lcd12864_ClearScreen();       //����
	}
}