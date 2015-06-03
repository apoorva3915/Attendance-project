#ifndef _8051_H_
#define _8051_H_

/* ------------------------------------------------------------------------- 
    SYSTEM 
   ------------------------------------------------------------------------- */
/* Cause delay in execution for the specified duration (in milliseconds). */
void Delaym(unsigned int time);


/* ------------------------------------------------------------------------- 
    SERIAL COMMUNICATION 
   ------------------------------------------------------------------------- */
/* Transmit data serially (at least a byte long). The first argument is the 
   pointer to the first byte to be transferred. The second argument is the 
   length of the transmission (in bytes). */
void Transmit(unsigned char *TOut, unsigned int TLen);

/* Receive data serially (at least a byte long). The first argument is the 
   pointer to where the first byte should be stored. The second argument is 
   the length of the transmission to be received (in bytes). */
void Receive(unsigned char *RIn, unsigned int RLen);

void InitCom();

#endif
