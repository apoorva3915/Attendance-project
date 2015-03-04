												//Attendance project using fingerprint sensor R305 module and GSM??? 

/*
* @author: Apoorva
* @Branch: XM1
 *
 */
#include<reg51.h>
//Function declarations
void cct_init(void);
void delay(int);
void lcdinit(void);
void writecmd(int);
void writedata(char);
void Return(void);
char READ_SWITCHES(void);
char get_key(void);
void lcd_display(char *disp);
void lcd_number(int);
//Pin description
/*
P2 is LCD
P3.0is RS
P3.2is E
P1.0 to P1.3 are keypad row outputs
P1.4 to P1.6 re keypad column inputs
*/
sbit RowA = P1^0;     
sbit RowB = P1^1;     
sbit RowC = P1^2;     
sbit RowD = P1^3;     
sbit C1   = P1^4;     
sbit C2   = P1^5;     
sbit C3   = P1^6;     

sbit E    = P3^2;     
sbit RS   = P3^0;     
//Main program
int main(void)
{
   char key , key2 ;  // key char for keeping record of pressed key
	int Q[9]={0};
	                

   cct_init();     // Make input and output pins as required         
   lcdinit();     // Initilize LCD

   writecmd(0x82);     
   lcd_display("ATTENDANCE");
   delay(100);
   writecmd(0xc2);
   delay(100);
   lcd_display("PROJECT");
   delay(100);
   while(1)

   { 
   
     key = get_key();   // Get pressed key  
	 writecmd(0x01);        
	 writedata(key);	// Echo the key pressed to LCD
	 //for(i=0;i<=10;++i)
	 Q[key-'1']++;
	   
	  if (key == 'A' )
	  {
	 writecmd(0x01);        
	  lcd_display("press any no.");
	   key = get_key();       
	 writecmd(0x01);
	 writedata(key);       
	  lcd_number(Q[key-'1']);
	 }       
   } 

}
 			 
void cct_init(void)
{

	P1 = 0xf0;   
	P2 = 0x00;   
	P3 = 0x00;      
}

void delay(int a)
{
   int i;
   for(i=0;i<a;i++);   
}

void writedata(char t)
{
   RS = 1;             
   P2 = t;             
   E  = 1;             
   delay(150);
   E  = 0;           
   delay(150);
}


void writecmd(int z) {
		RS = 0;             
		P2 = z;             
		E  = 1;             
		delay(150);
		E  = 0;             
		delay(150);
}

void lcdinit(void) {
  
		writecmd(0x38);    
		writecmd(0x0c);    
		writecmd(0x01);    
		writecmd(0x06);    
}

void Return(void) {
  writecmd(0x02);
    delay(1500);
}

char READ_SWITCHES(void)	
{	
	RowA = 0; RowB = 1; RowC = 1; RowD = 1; 	

	if (C1 == 0) { delay(10000); while (C1==0); return '1'; }
	if (C2 == 0) { delay(10000); while (C2==0); return '2'; }
	if (C3 == 0) { delay(10000); while (C3==0); return '3'; }


	RowA = 1; RowB = 0; RowC = 1; RowD = 1; 	

	if (C1 == 0) { delay(10000); while (C1==0); return '4'; }
	if (C2 == 0) { delay(10000); while (C2==0); return '5'; }
	if (C3 == 0) { delay(10000); while (C3==0); return '6'; }
	
	
	RowA = 1; RowB = 1; RowC = 0; RowD = 1; 	

	if (C1 == 0) { delay(10000); while (C1==0); return '7'; }
	if (C2 == 0) { delay(10000); while (C2==0); return '8'; }
	if (C3 == 0) { delay(10000); while (C3==0); return '9'; }

	
	RowA = 1; RowB = 1; RowC = 1; RowD = 0; 	

	if (C1 == 0) { delay(10000); while (C1==0); return 'c'; }
	if (C2 == 0) { delay(10000); while (C2==0); return 'R'; }
	if (C3 == 0) { delay(10000); while (C3==0); return 'A'; }


	return 'n';           	
	}


char get_key(void)           
{
	char key= 'n';              
    while(key == 'n')              
    key = READ_SWITCHES();  
		while(READ_SWITCHES()!='n');
		
    return key;                 
}

/*
 * Comments explaining this function
 */

void lcd_display(char *disp) {
		int x ;
		for(x = 0 ; disp[x]!= 0 ; x++) {
				writedata(disp[x]);
		}
}

void lcd_number(int a){
	
	int d=0;
	while(a){d=d*10+a%10;d/=10;}
	do
	{
		writedata(d%10+'0');
		d/=10;
	}while(d);
}

