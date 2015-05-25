#include <at89x51.h>
#include "8051.h"

/* Delaym() takes as argument the time (in milliseconds) for which to cause delay 
 * in execution. It uses Timer 0 for keeping time. Note that Delaym() assumes 
 * XTAL = 22.118 MHz. For any other XTAL, the actual delay will not match the 
 * intended delay. 
 */
void Delaym(unsigned int time) {
    unsigned int i;
    TMOD |= 0x01;   // Use Timer 0 in mode 1: 16-bit
    for (i = 0; i < time; i++) {
        TL0 = 0xCC;     // Load Timer 0 low byte with 0xCC
        TH0 = 0xF8;     // Load Timer 0 high byte with 0xF8
        TR0 = 1;        // Start Timer 0
        while (!TF0)    // Monitor the Timer 0 overflow flag for rise
            ;
        TR0 = 0;        // Stop Timer 0
        TF0 = 0;        // Clear the Timer 0 overflow flag
    }
}
