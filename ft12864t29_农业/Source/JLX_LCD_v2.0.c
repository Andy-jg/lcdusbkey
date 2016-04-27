/***************************************************************
 



����ѶLCDר������
�� �� ��������ѶLCDר������
�� д �ˣ�ifm
��дʱ�䣺2015��6��27��
Ӳ��֧�֣�STCϵ�е�Ƭ��
�ӿ�˵�����μ�ע��
�޸���־��
2015.7.30
�������º���
	LCDAddress,
	display0805,
	DisplayString,
	DisplayNumber,
	display1608,
	display1616,
	display3232,
	display6464,
	display19264,
���ǵ�display0805��������Ĺ���ʵ���Ǽ��ߣ��������Խ�Լϵͳ��Դ
2015.8.1
��������DisplayString1608,DisplayNumber1608
�޸ĺ�����DisplayString,DisplayNumberΪDisplayString0805,DisplayNumber0805
�ָ�display0805�Թ�DisplayNumber0805���á�
�޸�����ϸ��
2015.8.2
��������DrawPoint
2015.8.20
ȡ��Ƭѡ�ߡ����ǵ�һ�㲻��ͬʱ����������Ļ��ȡ��Ƭѡ�߽�Լ��Դ��
Ӳ����Ƭѡ��ֱ�ӽӵء�
��    ����V2.0
��    ע�������������ڷֱ���192*64��Һ�����������ֱ����������޸�
***************************************************************/

#include "string.h"
#include "JLX_LCD_v2.0.h"
#include "ASCII.h"
#include "ASCII16.h"
#include "chinese16_16.h"


#define delay()	{	_nop_();_nop_();_nop_();_nop_(); _nop_();_nop_(); 	\
				}


//1100, 100ms@12m				
void timer(unsigned int t)
{
    unsigned int i;
    unsigned char j;
    for(i=0; i<t; i++)
        for(j=0; j<200; j++);
}




//void testio(void)
//{
//	while(1)
//	{
//		SDAT  = SCLK =CD =RS =CS =LCD_LED = 1;
//		timer(10000);
//		SDAT  = SCLK =CD =RS =CS =LCD_LED = 0;
//		timer(10000);
//	}
//}


void wrLCD( unsigned char dat ,bit Cmd0_Dat1)
{
	unsigned char i;

	CD = Cmd0_Dat1?1:0;
	SCLK = 0; 
	for(i=0; i<8; i++)
    {
        
        SDAT = (dat&0x80) ;
        delay();//ͬ��
        SCLK = 1;
        delay();//ͬ��
		SCLK = 0;
		dat <<= 1;
    }
	SDAT = 0;
}

//д��Һ����ָ�write lcd command
void wlc(unsigned char dat)
{
    wrLCD(dat, 0);
}
//д��Һ�������ݣ�write lcd data
void wld(unsigned char dat)
{
    wrLCD(dat, 1);
}

//��ʼ��Һ����
void InitLCD()
{
	
	SDAT  = SCLK =CD =RS =CS =LCD_LED = 0;

    timer(30);//��λǰ����ʱ�������3ms    
    RS = 1;
//    timer(10);//��λ��ɺ�д���������ǰ����ʱ�������150ms
	
	wlc(0x2f);//0010 1111
	wlc(0x22);//0010 0010
	wlc(0x81);//1000 0001
	wlc(0x3f);//0011 1111
	wlc(0xa2);//1010 0010
	wlc(0xa1);//1010 0001
	wlc(0xc0);//1100 0000
	wlc(0x10);//0001 0000
	wlc(0x00);//0000 0000
	wlc(0xb0);//1011 0000
	wlc(0xaf);//1010 1111
	wlc(0xa6);//1010 0110
	//

//	CS =CD = 1;
//	timer(1);
//	CS  = 0;
}

//����
void CleanLCD()
{
	
    unsigned int i =0;
	unsigned int j=0; 
//	timer(4);
	for(j=0;j<8;j++)
	{ 
		LCD_LED= ~LCD_LED;
		
		wlc(0xb0+j);//1011 0000	//1011 0001 
		wlc(0x00);//0000 0000	//0000 0000
		wlc(0x10);//0001 0000	//0001 0000
		wlc(0x40);//0100 0000	//0100 0000
		
		for(i=0;i < 128+4;i++)
		{
			wld(0x00);
		}
	}

//	CS = 1;
//	timer(1);
//	CS = 0;

	for(j=0;j<8;j++)
	{ 
		LCD_LED= ~LCD_LED;
		
		wlc(0xb0+j);//1011 0000	//1011 0111 
		wlc(0x04);//0000 0100	 
		wlc(0x10);//0001 0000	 
		wlc(0x40);//0100 0000	 
		
		for(i=0;i < 128+4;i++)
		{
			wld(0x01);
		}
	}

	SDAT  = SCLK =CD  =LCD_LED =0;	 
//	CS = 1;
	while(1);
}



//����д���ַ��ҳ���о���0��ʼ
void LCDAddress(unsigned char page, unsigned char column)
{
    wlc(0xB0 | page);//Ҳ������0xB0 + page����λ�����ٶȱȼӷ��ٶȿ�
    wlc(0x0F & column);//�����еĵ���λ
    wlc(0x10 | column >> 4);//�����еĸ���λ����λ��������ȼ����ڻ�����
}
//��ָ�����л�һ���㣬�Ḳ��ԭ��������
void DrawPoint(unsigned char line, unsigned char column)
{
    LCDAddress(line>>3, column);
    wld(0x01 << (line & 0x07));
}
//��ָ��ҳ��ָ����д��8*5��ͼ������
void display0805(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i;
    
    LCDAddress(page, column);
    
    for(i=0; i<5; i++)
    {
        wld(*p);
        p++;
    }
}
//��ָ��ҳ��ָ����д��16*8��ͼ������
void display1608(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i, j;
    for(i=0; i<2; i++)
    {
        LCDAddress(page + i, column);
        for(j=0; j<8; j++)
        {
            wld(*p);
            p++;
        }
    }
}
//��ָ��ҳ��ָ����д��16*16��ͼ������
void display1616(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i, j;
    for(i=0; i<2; i++)
    {
        LCDAddress(page + i, column);
        for(j=0; j<16; j++)
        {
            wld(*p);
            p++;
        }
    }
}


//��ָ��ҳ��ָ����д��32*32��ͼ������
void display3232(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i, j;
    for(i=0; i<4; i++)
    {
		LCDAddress(page + i, column);
		for(j=0; j<32; j++)
		{
            wld(*p);
            p++;
        }
    }
}
//��ָ����д��64*64��ͼ������
void display6464(unsigned char column, unsigned char *p)
{
    unsigned char i, j;
    for(i=0; i<8; i++)
    {
        LCDAddress(i, column);
        for(j=0; j<64; j++)
        {
            wld(*p);
            p++;
        }
    }
}
void display19264(unsigned char *p)
{
    unsigned int i;
    LCDAddress(0, 0);
    
    for(i=0; i<1536; i++)
    {
        wld(*p);
        p++;
    }
}
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ0805��Ӣ���ַ���
void DisplayString0805(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i=0, j, k;
    
    LCDAddress(page, column);
    while(p[i])//�ж��ַ������Ƿ����
    {
        k = p[i] - 0x20;//��ȡ��ǰ�ַ���32���ֵ
        for(j=0; j<5; j++)
        {
            wld(ASCII[j + k * 5]);
		}
		wld(0x00);//���һ�пհף����ַ�����
		i++;
	}
}
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ0805������
void DisplayNumber0805(unsigned char page, unsigned char column, unsigned int a)
{
    char i=0, j;//ע�������õ���char������unsigned char
    for(i=4; i>=0; i--)//�ж������Ƿ����
    {
        LCDAddress(page, column + i * 6);
        j = a % 10 + 16;//��ȡ�����ּ�16���ֵ�����ֵ������������ھ����ASCII���е����
        display0805(page, column + i * 6, &ASCII[j*5]);
        wld(0x00);//���һ�пհף����ַ�����
        a /= 10;
    }
}
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ1608��Ӣ���ַ���
void DisplayString1608(unsigned char page, unsigned char column, unsigned char *p)
{
    unsigned char i=0, j;
    while(p[i])//�ж��ַ������Ƿ����
    {
		j = p[i] - 0x20;//��ȡ��ǰ�ַ���32���ֵ
		display1608(page, column + i*8, &ASCII16[j*16]);
		i++;
    }
}
//��ָ��ҳ��ָ������ʾ�ߴ�Ϊ1608������
void DisplayNumber1608(unsigned char page, unsigned char column, unsigned int a)
{
    char i=0, j;//ע�������õ���char������unsigned char
    for(i=4; i>=0; i--)//�ж����������Ƿ����
    {
		j = a % 10 + 16;//��ȡ�����ּ�16���ֵ�����ֵ������������ھ����ASCII���е����
		display1608(page, column + i * 8, &ASCII16[j * 16]);//����j*16�Ժ��������ͼ�ε�λ��
		a /= 10;
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
			display1616(page,column,&Chinese1616[GBKPos].ChineseMAP);
			column += 16;
		}
		p+=2;
	}
	
	
//	display1616(0,0,&zhongguo[2]);
//	display1616(0,32,&zhongguo[36]);
}
