#ifndef _R305_FINGERPRINT_H_
#define _R305_FINGERPRINT_H_

#define MAX_ACK_LENGTH 36
#define MAX_CMD_LENGTH 36

#define CMD_FAIL        0
#define CMD_SUCCESS     1
#define ENROL_FAIL      2
#define ENROL_SUCCESS   3
#define ERR_COMM        4
#define ERR_SCAN        5
#define ERR_SYS         6
#define ERR_TEMP        7
#define MATCH_FAIL      8
#define MATCH_SUCCESS   9

typedef unsigned short uint2;
typedef unsigned char uint1;

uint2 length(uint1 *DATA);
void cmdTransmit(uint1 *DATA);
uint1 *cmdReceive(uint2 lenDATA);

uint1 genImg();
uint1 genChar(uint1 buffID);
uint1 genTemp();

#endif
