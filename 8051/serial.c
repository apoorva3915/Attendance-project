#include <at89x51.h>
#include "8051.h"

/* The byte to be transmitted serially is placed in the SBUF register. A total of 
 * 10 bits are transferred (when using in Serial Mode 1), the first being the start 
 * bit followed by the 8 bit data and finally the stop bit. The 8051 raises the 
 * TI flag bit of the SCON register when the stop bit for the transmission is being 
 * transferred. The TI flag bit must be cleared before the next byte in SBUF can be 
 * transmitted. 
 * 
 * Transmit() can be used to transmit data serially which is at least a byte. 
 * It takes two arguments: the pointer to the first byte to be transferred, and the 
 * length of the transmission (in bytes). 
 */
void Transmit(unsigned char *TOut, unsigned int TLen) {
    unsigned int i;
    
    InitCom();
    TR1 = 1;        // Raise TR1 bit of TCON register to start Timer 1
    for (i = 0; i < TLen; i++) {
        TI = 0;                 // Clear TI bit of SCON register
        SBUF = *(TOut + i);     // Write to SBUF register the byte to be transmitted
        while (!TI)             // Monitor TI flag bit of SCON register
            ;
    }
    TR1 = 0;        // Clear the TR1 bit of TCON register to stop Timer 1
}

/* The serial communication is received with leading start bit and trailing stop 
 * bit. The internal circuitry of the 8051 strips the the incoming signal of the 
 * leading and trailing bits and deposits the 8-bit byte in the SBUF register. 
 * The 8051 raises the RI flag bit to indicate that SBUF has a byte. The RI flag 
 * bit must be cleared before the next byte can be placed in SBUF. 
 * 
 * Receive() can be used to receive data serially which is at least a byte in length. 
 * It takes two arguments: the pointer to the location where the first byte is to be 
 * stored, and the length of the transmission to be received (in bytes). 
 */
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

/* InitCom() is used to initialize 8051 for serial communication. It sets the 8051 
 * in Serial Mode 1: 8-bit data, 1 stop bit, 1 start bit. REN is raised. A baud rate 
 * of 57600 is used. Timer 1 is used to generate the baud rate. The SMOD bit of the 
 * PCON register (not bit-addressable) is raised to double the baud rate (without 
 * this a baud rate of 28800 would have been achieved). Note that this baud rate is 
 * achieved only with XTAL = 22.1184 MHz.
 */
void InitCom() {
    PCON |= 0x80;   // Raise SMOD bit of PCON to double the baud rate
    TMOD |= 0x20;   // Use Timer 1 in mode 2, 8-bit auto-reload
    TH1 = 0xFE;     // Set TH1 for 57600 baud rate when XTAL = 22.1184 MHz and SMOD is raised
    SCON = 0x50;    // Serial Mode 1, 8-bit data, 1 stop bit, 1 start bit, REN set
}
