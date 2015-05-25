#ifndef _R305_FINGERPRINT_H_
#define _R305_FINGERPRINT_H_

#define MAX_ACK_LENGTH 36
#define MAX_CMD_LENGTH 36

typedef unsigned short uint2;
typedef unsigned char uint1;

uint2 length(uint1 *DATA);
void cmdTransmit(uint1 *DATA);
uint1 *cmdReceive(uint2 lenDATA);

#endif
