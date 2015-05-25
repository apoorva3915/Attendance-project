#ifndef _8051_H_
#define _8051_H_

/* System */
void Delaym(unsigned int time);

/* Serial Communication */
void Transmit(unsigned char *TOut, unsigned int TLen);
void Receive(unsigned char *RIn, unsigned int RLen);

/* ------------------------------------------------------------------------
 * Functions for use within library routines 
 * ------------------------------------------------------------------------ */

/* Serial Communication */
void InitCom();

#endif
