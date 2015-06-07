#include<reg51.h>
#include<stdio.h>
#include<string.h>
#include<lcd_8051.h>


char response;
void move(void);           
void initial_lcd(void);
void string_to_lcd(unsigned char *s);
void write_lcd(unsigned char dat,unsigned int com);
void delay_ms(unsigned int x);


/////////////functions for the LCD---------------------------------------

void move(void){

	unsigned char n;
        for(n=0;n<20;n++);
        delay_ms(1);
	}

void initial_lcd(void){

	write_lcd(0x38,0);
	write_lcd(0x0c,0);
	write_lcd(0x01,0);
	}


void string_to_lcd(unsigned char *s){

	unsigned char i,l;
	l=strlen(s);
	for(i=0;i<l;i++){

                                write_lcd(*s,1);
                                delay_ms(1);
                                s++;
}
}

void write_lcd(unsigned char dat,unsigned int com){

	RS=com;
	LCD=dat;move();
	EN=1;move();
	EN=0;move();
	}

void delay_ms(unsigned int x)    // delays x msec (at 11.0592MHz)
{
        unsigned char j=0;
        while(x-- > 0){
        for (j=0; j<125; j++){;}
	}

 //timers
TMOD=0x01;
TH0=0x3c;
TL0=0xb0;
TR0=1;
while(!TF0);
TF0=0;
TR0=0;

}

