#include <at89x51.h>
#include "8051.h"

/* The outgoing byte is placed in the SBUF register. The 8051 raises the 
   TI flag when the last bit (stop bit) is being transferred. The TI flag 
   must be cleared before the next byte in SBUF can be transferred. */
void Transmit(unsigned char *TOut, unsigned int TLen) {
    unsigned int i;
    InitCom();
    TR1 = 1;        // Raise TR1 bit of TCON register to start Timer 1
    for (i = 0; i < TLen; i++) {
        TI = 0;                 // Clear TI bit of SCON register
        SBUF = *(TOut + i);     // Write to SBUF register the outgoing byte
        while (!TI)             // Monitor TI bit of SCON register
            ;
    }
    TR1 = 0;        // Clear TR1 bit of TCON register to stop Timer 1
}

/* The internal circuitry of the 8051 strips the the incoming signal of the 
   leading and trailing bits, and deposits the byte in the SBUF register. 
   The RI bit is raised to indicate that SBUF has a byte. The RI bit must be 
   cleared before the next byte can be placed in SBUF. */
void Receive(unsigned char *RIn, unsigned int RLen) {
    unsigned int i;
    InitCom();
    TR1 = 1;            // Raise TR1 bit of TCON register to start Timer 1
    for (i = 0; i < RLen; i++) {
        RI = 0;                 // Clear RI bit of SCON register
        while (!RI)             // Monitor RI flag bit of SCON register
            ;
        *(RIn + i) = SBUF;      // Read from SBUF register the byte that was received
    }
    TR1 = 0;        // Clear the TR1 bit of TCON register to stop Timer 1
}

/* Initialize the 8051 for serial communication in Serial Mode 1: 8-bit data, 
   1 stop bit, 1 start bit. REN is raised. Baud rate is 57600. Timer 1 is used 
   to generate baud rate. The SMOD bit of PCON register (not bit-addressable) 
   is raised to double the baud rate (without this a baud rate of 28800 would 
   have been achieved). 
   Note that this baud rate is achieved only with XTAL = 22.1184 MHz and when 
   SMOD is raised. */
void InitCom() {
    PCON |= 0x80;   // Raise SMOD bit of PCON to double the baud rate
    TMOD |= 0x20;   // Use Timer 1 in mode 2, 8-bit auto-reload
    TH1 = 0xFE;     // Set TH1 for 57600 baud rate
    SCON = 0x50;    // Serial Mode 1, 8-bit data, 1 stop bit, 1 start bit, REN set
}
