#include<reg51.h>
#include "r305-fingerprint.h"
#include<string.h>
#include <stdio.h>

char response;
void move(void);           
void initial_lcd(void);
void string_to_lcd(unsigned char *s);
void write_lcd(unsigned char dat,unsigned int com);
void delay_ms(unsigned int x);

//pin description for the lcd interface --- 4 bit mode

sbit D4=P1^2;
sbit D5=P1^3;
sbit D6=P1^4;
sbit D7=P1^5;

sbit EN=P1^6;//Enable pin is for starting or enabling the module. A high
             //to low pulse of about 450ns pulse is given to this pin.

sbit RS=P1^7;//RS is the register select pin used to write display
             //data to the LCD (characters), this pin has to be high
             //when writing the data to the LCD. During the initializing
             //sequence and other commands this pin should low.
 
sbit SW_ADD        = P1^0;
sbit SW_SEARCH     = P1^1;

sbit LED = P3^5 ;

 
 
sbit FP_ADD        = P3^2;
sbit FP_EMPTY       = P3^3;
sbit FP_SEARCH      = P3^4;
 
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
}



void main()
{
/*To use the 8051's on-board serial port, it is generally necessary to initialize the following
SFRs: SCON, TCON, and TMOD. This is because SCON controls the serial port. However, in most cases the
program will wish to use one of the timers to establish the serial port's baud rate. In this case, it is necessary to
configure timer 1 by initializing TCON and TMOD.
*/
    //  Intialize variables 
    response = 0;
         
    //  Intialise Serial Port 
    //Sets up MCU to use 9600 bps @ 11.059 MHz Crystal
    SCON = 0x52; //0011 0010, 8-bit UART mode , serial control , This SFR controls the baud rate of the serial
                 //port, whether the serial port is activated to receive data, and also contains flags that are set when a byte
                 //is successfully sent or received.
    TMOD = 0x20; //0010 0000 [timer 1 , mode 2] 8-bit auto reload timer/counter; THx(timer high) holds a value which is to be reloaded TLx(timer low) each time it overfolws , {mazidi , page no. 317}
    TH1= 0xfd;   // 9600 8-n-1 {mazidi pgno. 336}
    TR1 = 1; // run timer1
     
    //  Program Loop 
    while(1)
    {
         
        P0=0xff;
        P1=0xff;
        delay_ms(100);
        initial_lcd();
        while(1){
	  
        write_lcd(0x81,0);
        string_to_lcd("HELLO");
        write_lcd(0xc4,0);
        string_to_lcd("EVERYONE");
        write_lcd(0x01,0);
        delay_ms(100);
	
	
        write_lcd(0x81,0);
        string_to_lcd("PLACE YOUR");
        write_lcd(0xc4,0);
        string_to_lcd("FINGER")
        write_lcd(0x01,0);
        delay_ms(100);


	LED = 0 ;
	response = scan() ;
	if (response == CMD_SUCCESS){
	
	write_lcd(0x81,0);
        string_to_lcd("FINGER");
        write_lcd(0xc4,0);
        string_to_lcd("DETECTED");
        write_lcd(0x01,0);
        delay_ms(100);

	LED = 1 ;
	}
	
	else {
	write_lcd(0x81,0);
        string_to_lcd("ERROR");
        write_lcd(0xc4,0);
        string_to_lcd("TRY AGAIN");
        write_lcd(0x01,0);
        delay_ms(100);
	
//////When the finger is detected , two options either to auth or enrol 
///////// Add           
            if(SW_ADD==0)   // check for Add switch
            {               
                // Trigger Add Function
                FP_ADD = 0;
                delay_ms(50);
                FP_ADD = 1;
                 
               
                response = enrol();
                if(response==ENROL_SUCCESS) 
                {
                    
                    write_lcd(0xc4,0);
                    string_to_lcd("SUCCESS");
                    write_lcd(0x01,0);
                    delay_ms(100);
                } else
                {                   
                 

                    write_lcd(0xc4,0);
                    string_to_lcd("ERROR");
                    write_lcd(0x01,0);
                    delay_ms(100);
            }
             

                     
    } // end while
}// end main
