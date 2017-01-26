#ifndef __OLED_H_
#define __OLED_H_

void OLED_Init(void);
void OLED_Fill(unsigned char bmp_dat);	  
//void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P14x16Ch(unsigned char x,unsigned char y,unsigned char N);
void OLED_P16x32(unsigned char x,unsigned char y,unsigned char N);



#endif