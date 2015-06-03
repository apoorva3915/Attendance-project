#ifndef _R305_FINGERPRINT_H_
#define _R305_FINGERPRINT_H_

#define CMD_FAIL        0x00
#define CMD_SUCCESS     0x01
#define ENROL_FAIL      0x02
#define ENROL_SUCCESS   0x03
#define ERR_COMM        0x04
#define ERR_SCAN        0x05
#define ERR_SYS         0x06
#define ERR_TEMP        0x07
#define MATCH_FAIL      0x08
#define MATCH_SUCCESS   0x09

typedef unsigned short uint2;
typedef unsigned char uint1;

void cmdTransmit(uint1 *DATA, uint2 lenDATA);
uint1 *cmdReceive(uint2 lenDATA);
uint1 getNTemp(uint2 *nTemp);
uint1 clrLib(void);

uint1 genImg(void);
uint1 genChar(uint1 buffID);
uint1 genTemp(void);
uint1 strTemp(uint1 buffID);
uint1 srchLib(uint1 buffID, uint2 *pageID);

uint1 scan(void);
uint1 auth(uint2 *uID);
uint1 enrol(void);

#endif
