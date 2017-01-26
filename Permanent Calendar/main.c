#include "reg52.h"
#include "OLED12864.h"
#include "intrins.h"

//unsigned int DS18B20_GetTemp();	  //¶ÁÈ¡ÎÂ¶È


sbit DQ = P1^7;
sbit CLK=P2^1;
sbit SDA=P2^0;
sbit RST=P2^4;

sbit K1=P3^4;
sbit K2=P3^5;
sbit K3=P3^6;
sbit K4=P3^7;

sbit FM=P2^3;

unsigned char GB = 0,GB2 = 0;
unsigned char NZ_1_hour=8,NZ_1_min=30,NZ_2_hour=14,NZ_2_min=30;
bit NZ_1 = 0,NZ_2 = 0;

void DS1302_Wrbyte(unsigned char add,unsigned char byt)
{
	unsigned char i;
	RST = 0;
	CLK = 0;
	RST = 1;
	for(i=0;i<8;i++)
	{
		add >>= 1;
		SDA = CY;
		CLK = 1;
		CLK = 0;
	}
	for(i=0;i<8;i++)
	{
		byt >>= 1;
		SDA = CY;
		CLK = 1;
		CLK = 0;
	}
	RST = 0;
}

unsigned char DS1302_Rebyte(unsigned char add)
{
	unsigned char i,byt;
	RST = 0;
	CLK = 0;
	RST = 1;
	for(i=0;i<8;i++)
	{
		add >>= 1;
		SDA = CY;
		CLK = 1;
		CLK = 0;		
	}
	for(i=0;i<8;i++)
	{		
		byt>>=1;
		if(SDA == 1)
			byt|=0x80;
		CLK = 1;
		CLK = 0;
	}
    RST = 0;

	return byt;	
}

unsigned char byte1,byte2,byte3,byte4,byte5;

void delay18ms(void)   
{
    unsigned char a,b;
    for(b=155;b>0;b--)
        for(a=52;a>0;a--);
}

void delay40us(void)   
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=4;a>0;a--);
}

void delay30us(void)  
{
    unsigned char a;
    for(a=12;a>0;a--);
}

unsigned char DHT11_Rebyte()
{
	unsigned char dat,j,i =2;
	for(j=0;j<8;j++)
	{
		while((!DQ)&&i++);
		delay30us();
		if(DQ ==0)
			dat = dat<<1;
		else
			dat = (dat<<1)|0x01;
		i = 2;
		while((DQ)&&i++);
	}	
	return dat;
}


void DHT11_ReTemp()
{
	unsigned char i=2;
	DQ = 1;	
	DQ = 0;
	delay18ms();
	DQ = 1;
	delay40us();
	if(DQ == 0)
	{
		while((!DQ)&&i++);
		i=2;
		while((DQ)&&i++);
		byte1 = DHT11_Rebyte();
		byte2 = DHT11_Rebyte();
		byte3 = DHT11_Rebyte();
		byte4 = DHT11_Rebyte();
		byte5 = DHT11_Rebyte();
	}
}

void Init()
{
	
	OLED_P14x16Ch(16,0,2);
	OLED_P14x16Ch(40,0,3);
	OLED_P14x16Ch(56,0,4);
	OLED_P14x16Ch(112,0,15);
	OLED_P16x32(48,2,10);
	OLED_P14x16Ch(40,6,12);
	OLED_P14x16Ch(72,6,13);
	OLED_P14x16Ch(104,6,11);
	OLED_P8x16Str(8,6,"20");
	DS1302_Wrbyte(0x80,DS1302_Rebyte(0x81)&0x7f);
	DS1302_Wrbyte(0x84,DS1302_Rebyte(0x85)&0x7f);

	FM = 1;

}

void xs_shuzi(unsigned char x,unsigned char y,unsigned int i)
{
	switch(i)
	{
		case 0:	OLED_P8x16Str(x,y,"0");
				break;
		case 1:	OLED_P8x16Str(x,y,"1");
				break;
		case 2:	OLED_P8x16Str(x,y,"2");
				break;
		case 3:	OLED_P8x16Str(x,y,"3");
				break;
		case 4:	OLED_P8x16Str(x,y,"4");
		break;
		case 5:	OLED_P8x16Str(x,y,"5");
		break;
		case 6:	OLED_P8x16Str(x,y,"6");
		break;
		case 7:	OLED_P8x16Str(x,y,"7");
		break;
		case 8:	OLED_P8x16Str(x,y,"8");
		break;
		case 9:	OLED_P8x16Str(x,y,"9");
		break;
	}
}


void xs_Temp()
{
	DHT11_ReTemp();
	xs_shuzi(96,0,byte1/10);
	xs_shuzi(104,0,byte1%10);
	xs_shuzi(0,0,byte3/10);
	xs_shuzi(8,0,byte3%10);
}


void xs_Time()
{
	xs_shuzi(96,4,DS1302_Rebyte(0x81)>>4);
	xs_shuzi(104,4,DS1302_Rebyte(0x81)&0x0f);
	
	OLED_P16x32(64,2,(DS1302_Rebyte(0x83)<<1)>>5);
	OLED_P16x32(80,2,DS1302_Rebyte(0x83)&0x0f);
	
	OLED_P16x32(16,2,DS1302_Rebyte(0x85)>>4);
	OLED_P16x32(32,2,DS1302_Rebyte(0x85)&0x0f);
	
	xs_shuzi(24,6,DS1302_Rebyte(0x8d)>>4);
	xs_shuzi(32,6,DS1302_Rebyte(0x8d)&0x0f);
	
	xs_shuzi(56,6,DS1302_Rebyte(0x89)>>4);
	xs_shuzi(64,6,DS1302_Rebyte(0x89)&0x0f);
	
	xs_shuzi(88,6,DS1302_Rebyte(0x87)>>4);
	xs_shuzi(96,6,DS1302_Rebyte(0x87)&0x0f);
	
	OLED_P14x16Ch(72,0,DS1302_Rebyte(0x8b)+4);	
}

void delay10ms(void)   
{
    unsigned char a,b;
    for(b=249;b>0;b--)
        for(a=17;a>0;a--);
}


unsigned char sm_K1()
{
	if(K1 == 0)
	{
		delay10ms();
		if(K1 == 0)
		{
			while(!K1);
			return 1;
		}
		return 1;	
	}
	else
		return 0;
}

unsigned char sm_K2()
{
	if(K2 == 0)
	{
		delay10ms();
		if(K2 == 0)
		{
			while(!K2);
			return 1;
		}
		return 1;	
	}
	else
		return 0;
}

unsigned char sm_K3()
{
	if(K3 == 0)
	{
		delay10ms();
		if(K3 == 0)
		{
			while(!K3);
			return 1;
		}
		return 1;	
	}
	else
		return 0;
}

unsigned char sm_K4()
{
	if(K4 == 0)
	{
		delay10ms();
		if(K4 == 0)
		{
			while(!K4);
			return 1;
		}
		return 1;	
	}
	else
		return 0;
}

void Szsz_xs()
{
	OLED_P14x16Ch(0,0,16);
	OLED_P14x16Ch(16,0,17);
	OLED_P14x16Ch(32,0,18);
	OLED_P14x16Ch(48,0,19);

	OLED_P14x16Ch(80,2,3);
	OLED_P14x16Ch(96,2,4);

	OLED_P14x16Ch(40,6,12);
	OLED_P14x16Ch(72,6,13);
	OLED_P14x16Ch(104,6,11);

	OLED_P8x16Str(16,2,":");
	OLED_P8x16Str(40,2,":");
	
	xs_shuzi(48,2,DS1302_Rebyte(0x81)>>4);
	xs_shuzi(56,2,DS1302_Rebyte(0x81)&0x0f);
	
	xs_shuzi(24,2,(DS1302_Rebyte(0x83)<<1)>>5);
	xs_shuzi(32,2,DS1302_Rebyte(0x83)&0x0f);
	
	xs_shuzi(0,2,DS1302_Rebyte(0x85)>>4);
	xs_shuzi(8,2,DS1302_Rebyte(0x85)&0x0f);
	
	OLED_P8x16Str(8,6,"20");	

	xs_shuzi(24,6,DS1302_Rebyte(0x8d)>>4);
	xs_shuzi(32,6,DS1302_Rebyte(0x8d)&0x0f);
	
	xs_shuzi(56,6,DS1302_Rebyte(0x89)>>4);
	xs_shuzi(64,6,DS1302_Rebyte(0x89)&0x0f);
	
	xs_shuzi(88,6,DS1302_Rebyte(0x87)>>4);
	xs_shuzi(96,6,DS1302_Rebyte(0x87)&0x0f);
	
	OLED_P14x16Ch(112,2,DS1302_Rebyte(0x8b)+4);	
}

void Qgb()
{
	OLED_P14x16Ch(0,4,21);
	OLED_P14x16Ch(14,4,21);
	OLED_P14x16Ch(28,4,21);
	OLED_P14x16Ch(42,4,21);
	OLED_P14x16Ch(56,4,21);
	OLED_P14x16Ch(70,4,21);
	OLED_P14x16Ch(84,4,21);
	OLED_P14x16Ch(98,4,21);
	OLED_P14x16Ch(112,4,21);
	OLED_P14x16Ch(126,4,21);
}

void Sz_Time()
{
	switch(GB)
	{
		case 0: Qgb();OLED_P14x16Ch(0,4,20);break;
		case 1: Qgb();OLED_P14x16Ch(24,4,20);break;
		case 2: Qgb();OLED_P14x16Ch(48,4,20);break;
		case 3: Qgb();OLED_P14x16Ch(112,4,20);break;
		case 4: Qgb();OLED_P14x16Ch(24,4,22);break;
		case 5: Qgb();OLED_P14x16Ch(56,4,22);break;
		case 6: Qgb();OLED_P14x16Ch(88,4,22);break;
	}
	if(sm_K2())
	{
		if(GB == 0)
			GB = 6;
		else
			GB--;
	}
	if(sm_K3())
	{
		if(GB == 6)
			GB = 0;
		else
			GB++;
	}
	if(sm_K4())
	{
		switch(GB)
		{
			case 0: {
						if(DS1302_Rebyte(0x85) >= 0x23)
							DS1302_Wrbyte(0x84,0x00);
						else
						{if((DS1302_Rebyte(0x85)==0x09)||(DS1302_Rebyte(0x85)==0x19))DS1302_Wrbyte(0x84,DS1302_Rebyte(0x85) + 0x07);
					   	 else DS1302_Wrbyte(0x84,DS1302_Rebyte(0x85) + 1);}	
					};break;

			case 1: {
						if(DS1302_Rebyte(0x83) >= 0x59)
							DS1302_Wrbyte(0x82,0x00);
						else
						{if((DS1302_Rebyte(0x83)&0x0f)==0x09)DS1302_Wrbyte(0x82,DS1302_Rebyte(0x83) + 0x07);
					   	 else DS1302_Wrbyte(0x82,DS1302_Rebyte(0x83) + 1);}	
					};break;

			case 2: {
						DS1302_Wrbyte(0x80,0x00);	
					};break;

			case 3: {
						if(DS1302_Rebyte(0x8b) >= 7)
							DS1302_Wrbyte(0x8a,0x01);
						else
							DS1302_Wrbyte(0x8a,DS1302_Rebyte(0x8b) + 1);	
					};break;

			case 4: {
						if(DS1302_Rebyte(0x8d) >= 0x20)
							DS1302_Wrbyte(0x8c,0x00);
						else
						{if((DS1302_Rebyte(0x8d)&0x0f)==0x09)DS1302_Wrbyte(0x8c,DS1302_Rebyte(0x8d) + 0x07);
						else DS1302_Wrbyte(0x8c,DS1302_Rebyte(0x8d) + 1);}	
					};break;

			case 5: {
						if(DS1302_Rebyte(0x89) >= 0x12)
							DS1302_Wrbyte(0x88,0x01);
						else
						{if((DS1302_Rebyte(0x89)==0x09))DS1302_Wrbyte(0x88,DS1302_Rebyte(0x89) + 0x07);
					   	 else DS1302_Wrbyte(0x88,DS1302_Rebyte(0x89) + 1);}	
					};break;

			case 6: {
						if(DS1302_Rebyte(0x87) >= 0x31)
							DS1302_Wrbyte(0x86,0x01);
						else
						{if((DS1302_Rebyte(0x87)&0x0f)==0x09)DS1302_Wrbyte(0x86,DS1302_Rebyte(0x87) + 0x07);
						else DS1302_Wrbyte(0x86,DS1302_Rebyte(0x87) + 1);}	
					};break;
		}	
	}
}

void NZSZ()
{
	OLED_P14x16Ch(0,0,23);
	OLED_P14x16Ch(16,0,17);
	OLED_P14x16Ch(32,0,18);
	OLED_P14x16Ch(48,0,19);	

	xs_shuzi(8,2,NZ_1_hour/10);
	xs_shuzi(16,2,NZ_1_hour%10);
	OLED_P8x16Str(24,2,":");
	xs_shuzi(32,2,NZ_1_min/10);
	xs_shuzi(40,2,NZ_1_min%10);


	xs_shuzi(8,6,NZ_2_hour/10);
	xs_shuzi(16,6,NZ_2_hour%10);
	OLED_P8x16Str(24,6,":");
	xs_shuzi(32,6,NZ_2_min/10);
	xs_shuzi(40,6,NZ_2_min%10);



	if(NZ_1==0)
	{
		OLED_P14x16Ch(80,2,26);
		OLED_P14x16Ch(96,2,27);	
	}
	else
	{
		OLED_P14x16Ch(80,2,24);
		OLED_P14x16Ch(96,2,25);
	}
	if(NZ_2==0)
	{
		OLED_P14x16Ch(80,6,26);
		OLED_P14x16Ch(96,6,27);	
	}
	else
	{
		OLED_P14x16Ch(80,6,24);
		OLED_P14x16Ch(96,6,25);
	}


	switch(GB2)
	{
		case 0: Qgb();OLED_P14x16Ch(8,4,20);break;
		case 1: Qgb();OLED_P14x16Ch(32,4,20);break;
		case 2: Qgb();OLED_P14x16Ch(88,4,20);break;
		case 3: Qgb();OLED_P14x16Ch(8,4,22);break;
		case 4: Qgb();OLED_P14x16Ch(32,4,22);break;
		case 5: Qgb();OLED_P14x16Ch(88,4,22);break;
	}

	if(sm_K2())
	{
		if(GB2 == 0)
			GB2 = 5;
		else
			GB2--;
	}
	if(sm_K3())
	{
		if(GB2 == 5)
			GB2 = 0;
		else
			GB2++;
	}

	if(sm_K4())
	{
		switch(GB2)
		{
			case 0: {if(NZ_1_hour==23) NZ_1_hour=0;else NZ_1_hour++;}break;
			case 1: {if(NZ_1_min==59) NZ_1_min=0;else NZ_1_min++;}break;
			case 2: NZ_1=!NZ_1;break;
			case 3: {if(NZ_2_hour==23) NZ_2_hour=0;else NZ_2_hour++;}break;
			case 4: {if(NZ_2_min==59) NZ_2_min=0;else NZ_2_min++;}break;
			case 5: NZ_2=!NZ_2;break;
		}
	}

	
}

void naozhong()
{
	if(NZ_1 == 1)
	{
		if(((DS1302_Rebyte(0x85)>>4)==(NZ_1_hour/10))&&((DS1302_Rebyte(0x85)&0x0f)==(NZ_1_hour%10)))
		{
			if(((DS1302_Rebyte(0x83)<<1>>5)==(NZ_1_min/10))&&((DS1302_Rebyte(0x83)&0x0f)==(NZ_1_min%10)))
				FM = 0;
			else 
				FM = 1;
		}
	}

	if(NZ_2 == 1)
	{
		if(((DS1302_Rebyte(0x85)>>4)==(NZ_2_hour/10))&&((DS1302_Rebyte(0x85)&0x0f)==(NZ_2_hour%10)))
		{
			if(((DS1302_Rebyte(0x83)<<1>>5)==(NZ_2_min/10))&&((DS1302_Rebyte(0x83)&0x0f)==(NZ_2_min%10)))
				FM = 0;
			else 
				FM = 1;
		}
	}
}

void delay6s(void)   
{
    unsigned char a,b,c;
    for(c=212;c>0;c--)
        for(b=160;b>0;b--)
            for(a=80;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}


void KJDH()
{
	OLED_P14x16Ch(0,0,28);
	OLED_P14x16Ch(16,0,29);
	OLED_P14x16Ch(32,0,30);
	OLED_P14x16Ch(48,0,31);
	OLED_P14x16Ch(64,0,32);
	
	OLED_P14x16Ch(48,3,33);
	OLED_P14x16Ch(64,3,34);
	OLED_P14x16Ch(80,3,35);
	OLED_P14x16Ch(96,3,36);
	OLED_P14x16Ch(112,3,37);
	
	OLED_P14x16Ch(32,6,38);
	OLED_P14x16Ch(48,6,39);
	OLED_P14x16Ch(64,6,40);
	OLED_P14x16Ch(80,6,41);	

	delay6s();
}

void main()
{											  	           	
		OLED_Init();
		KJDH();
XianShi:OLED_Fill(0x00);
		Init();
		while(1)
		{
			xs_Temp();
			xs_Time();
			if(sm_K1())
				goto SheZhi;
			if((NZ_1==1)||(NZ_2==1))
				OLED_P14x16Ch(96,2,14);
			else
				OLED_P14x16Ch(96,2,21);
				
			naozhong();			
		}
SheZhi:	OLED_Fill(0x00);
		FM = 1;		
		while(1)
		{			
			if(sm_K1())
				goto NaoZ;
			
			Szsz_xs();
			Sz_Time();	
		}
NaoZ:	OLED_Fill(0x00);
		while(1)
		{
			if(sm_K1())
				goto XianShi;
			NZSZ();
					
		}
}
