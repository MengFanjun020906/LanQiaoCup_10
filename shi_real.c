#include<reg51.h>
#include<intrins.h>
#include<iic.c>

sbit R1=P3^0;
sbit R2=P3^1;
sbit R3=P3^2;
sbit R4=P3^3;

//////////////////////////// 0    1     2    3    4    5    6   7    8    9    -   全灭   .
unsigned char code shuzi[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0xbf,0xff,0x7f};
/////////////////////////// A     b   C     d    E    F    H   L     P    U   n
unsigned char code zimu[]={0x88,0x83,0xc6,0xa1,0x86,0x8e,0x89,0xc7,0x8c,0xc1,0xc8};
////////////////////////////  0     1     2    3    4    5    6    7   
unsigned char code weizhi[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00};
//////////////////////////   1    2   3    4    5    6    7    8    灭
unsigned char ledweizhi[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};


#define uchar unsigned char
#define uint unsigned int
	
int addata;
long adc_dat;
unsigned int voltage;
int dat;
int v;
unsigned int count_f = 0;
unsigned char count_t = 0;
unsigned int dat_f = 0;
void display_v();
void display_voltage_led();
void Read_AIN3();
void Scan_S7();
void Scan_S6();
void Scan_S5();
void Scan_S4();
void shumaguan_shuzi(uchar a,uchar b)
{
	delay_ms(1);
	P2=(P2&0X1f)|0xC0;P0=weizhi[a];
	P2=(P2&0X1f)|0xE0;P0=shuzi[b];
	delay_ms(1);
	//消隐
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x00;
	P2 = P2 & 0x1f;
}
//数码管字母显示
void shumaguan_zimu(uchar a,uchar b)
{
	delay_ms(1);
	P2=(P2&0X1f)|0xC0;P0=weizhi[a];
	P2=(P2&0X1f)|0xE0;P0=zimu[b];
	delay_ms(1);
	//消隐
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x00;
	P2 = P2 & 0x1f;
}
void Init_Keys()
{
	R1=R2=R3=R4=1;
}
void Init_Timer()
{
	TH0 = 0xff;        
	TL0 = 0xff;
	
	TH1 = (65536 - 50000) / 256;        
	TL1 = (65536 - 50000) % 256;
	TMOD = 0x16;     
  ET0 = 1;
  ET1 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}
 
void Service_T0() interrupt 1
{
	count_f++;
}
 
void Service_T1() interrupt 3
{
  TH1 = (65536 - 50000) / 256;        
	TL1 = (65536 - 50000) % 256;
	count_t++;
	if(count_t == 20)
	{
		dat_f = count_f;
		count_f = 0;
		count_t = 0;
	}
}

void display_f()
{
	
	shumaguan_zimu(0,5);
	ledlight(1);
	if(dat_f>9999)
		shumaguan_shuzi(3,dat_f/1000);
	if(dat_f>999)
		shumaguan_shuzi(4,dat_f/1000%10);
	if(dat_f>99)
		shumaguan_shuzi(5,dat_f/100%10);
	if(dat_f>9)
		shumaguan_shuzi(6,dat_f/10%10);
		
		shumaguan_shuzi(7,dat_f%10);
	
	if(1000<=dat_f&&dat_f<5000)
	{
		ledlight(3);
	}
	else if(dat_f>=10000)
	{
		ledlight(3);
	}
}
void display_voltage_led()
{
	
	voltage=dat*100/51;
	
	P2=0x80;
	P0=0xee;//L1 and L5
	shumaguan_shuzi(6,voltage%100/10);
	shumaguan_shuzi(7,voltage%10);
	shumaguan_zimu(0,9);
	P2=(P2&0X1f)|0xC0;P0=weizhi[5];
	P2=(P2&0X1f)|0xFF;P0=shuzi[voltage/100]+0x80;
	v=dat*100/51;
	
	
	
}
void Read_AIN3()
{
	
	IIC_Start();				
	IIC_SendByte(0x90); 	
	IIC_WaitAck();  			
	IIC_SendByte(0x03); 	
	IIC_WaitAck();  							
	IIC_Stop(); 							
	
	IIC_Start();										
	IIC_SendByte(0x91); 			
	IIC_WaitAck(); 				
	dat = IIC_RecByte();				
	IIC_WaitAck(); 						
	IIC_Stop(); 						
}
void Scan_S7()
{
	
	uchar temp=4;
	
	if(R1==0)
	{
		delay_ms(50);
		if(R1==0)
		{
			temp++;
			while(R1==0);
		}
	}
	if(temp%2==0)
	{
		
	}
	if(temp%2==1)
	{
		while(1)
		{
			
		shumaguan_shuzi(0,11);
		shumaguan_shuzi(1,11);
		shumaguan_shuzi(2,11);
		shumaguan_shuzi(3,11);
		shumaguan_shuzi(4,11);
		shumaguan_shuzi(5,11);
		shumaguan_shuzi(6,11);
		shumaguan_shuzi(7,11);
		 
		if(v!=200)
		{
			P2=0x80;
			P0=0xee;//L1 and L5
		}
		
			if(R1==0)
			{
				delay_ms(50);
				if(R1==0)
				{
					break;
					while(R1==0);
				}
			}
		}
	}
}
void Scan_S6()
{
	
	uchar temp=4;
	
	if(R2==0)
	{
		delay_ms(50);
		if(R2==0)
		{
			temp++;
			while(R2==0);
		}
	}
	if(temp%2==0)
	{
		Read_AIN3(); 
		display_voltage_led();
	}
	if(temp%2==1)
	{
		
		while(1)
		{
		ledlight(8);
		Read_AIN3();
		display_v();
			if(R2==0)
			{
				delay_ms(50);
				if(R2==0)
				{
					break;
					while(R2==0);
				}
			}
		}
			
			
		}
	}
void Scan_S4()
{
	
	uchar temp=4;
	
	if(R4==0)
	{
		delay_ms(50);
		if(R4==0)
		{
			temp++;
			while(R4==0);
		}
	}
	if(temp%2==0)
	{
		Read_AIN3(); 
		display_voltage_led();
	}
	if(temp%2==1)
	{
		
		while(1)
		{
			display_f();
			
			if(R4==0)
			{
				delay_ms(50);
				if(R4==0)
				{
					break;
					while(R4==0);
				}
			}
		}
			
			
		}
	}

void display_v()
{
	voltage=dat*100/51;
	shumaguan_shuzi(6,voltage%100/10);
	shumaguan_shuzi(7,voltage%10);
	shumaguan_zimu(0,9);
	P2=(P2&0X1f)|0xC0;P0=weizhi[5];
	P2=(P2&0X1f)|0xFF;P0=shuzi[voltage/100]+0x80;
}
void Scan_S5()
{
	
	uchar temp=4;
	
	if(R3==0)
	{
		delay_ms(50);
		if(R3==0)
		{
			temp++;
			while(R3==0);
		}
	}
	if(temp%2==0)
	{
		Read_AIN3(); 
		display_voltage_led();
		
	}
	if(temp%2==1)
	{
		
		while(1)
		{
			dat=102;
			display_v();
			P2=0x80;
			P0=0xfa;//L1 and L3
			
			if(R3==0)
			{
				delay_ms(50);
				if(R3==0)
				{
					break;
					while(R3==0);
				}
			}
		}
			
			
		}
	}
void main()
{
	Init_Keys();
	Init_Timer();
	while(1)
	{
		Read_AIN3();
		display_voltage_led();
		Scan_S7();
		Scan_S6();
		Scan_S5();
		Scan_S4();
	}
}
