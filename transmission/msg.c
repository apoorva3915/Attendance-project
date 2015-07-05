#include <reg51.h>
#include <stdio.h>
#define LED P0



void serial_init(void);
unsigned int j;
//Setup the serial port for 9600 baud at 11.0592MHz.
//-------------------------------------------------
void serial_init(void)
{
SCON = 0x50;
/* SCON: mode 1, 8-bit UART, enable rcvr
*/
TMOD |= 0x20;
/* TMOD: timer 1, mode 2, 8-bit reload
*/
TH1 = 0xFD;
/* TH1: reload value for 9600 baud @ 11.0592MHz*/
TR1 = 1;
/* TR1: timer 1 run
*/
TI = 1;
/* TI: set TI to send first char of UART
*/
}
//Delay Routine start here
void delay1(int n)
	{
	int i;
	for(i=0;i<n;i++);
	}
void delay2(int n)
	{
	int i;
	for(i=0;i<n;i++)
	delay1(1000);
	}
void led_left()
	{
	for (j=0x01; j<=0x80; j<<=1)
		{
		LED = j;
		delay1(1000);
		}
	}
//-------------------------------------
//
Main program starts here
//-------------------------------------
void main(void)
{
serial_init();
//serial initialization
LED = 0x00;
printf("AT+CMGF=1%c",13);
delay2(20); //Text Mode

)
printf("AT+CMGS=\"9600292363\"%c",13); delay2(20); //Type your mobile number Eg
: "94257645050"
led_left();
//scroll left
delay1(20);
printf("Hi :) GSM Modem Test");
delay2(20); 
printf("%c",0x1A);
delay2(20); 
while(1);
}
