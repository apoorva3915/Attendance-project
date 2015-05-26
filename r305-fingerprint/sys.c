#include "../8051/8051.h"
#include "r305-fingerprint.h"

uint1 modAddress[] = {0xFF, 0xFF, 0xFF, 0xFF};

uint2 length(uint1 *DATA) {
    uint2 len;

    for (len = 0; *(DATA + len) != '\0'; len++)
        ;
    return len;
}

void cmdTransmit(uint1 *DATA) {
    uint1 cmd[MAX_CMD_LENGTH];
    uint2 checksum = 0x01, len, i;

    cmd[0] = 0xEF;
    cmd[1] = 0x01;
    for (i = 0; i < 4; i++)
        cmd[i + 2] = modAddress[i];
    cmd[6] = 0x01;

    len = length(DATA) + 2;
    cmd[8] = len;
    len = len >> 8;
    cmd[7] = len;
    len = cmd[8] + (cmd[7] << 8);
    checksum += cmd[7] + cmd[8];

    for (i = 0; *(DATA + i) != '\0'; i++) {
        cmd[i + 9] = *(DATA + i);
        checksum += *(DATA + i);
    }
    
    i += 9;
    cmd[++i] = checksum;
    checksum = checksum >> 8;
    cmd[--i] = checksum;

    Transmit(cmd, 9 + len);
}

uint1 *cmdReceive(uint2 lenDATA) {
    uint1 ack[MAX_ACK_LENGTH], DATA[lenDATA + 1];
    uint2 i;

    Receive(ack, 11 + lenDATA);
    for (i = 0; i < lenDATA; i++) {
        DATA[i] = ack[9 + i];
    }
    DATA[i] = '\0';
    
    return DATA;
}
