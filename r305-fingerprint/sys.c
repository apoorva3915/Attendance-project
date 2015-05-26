#include "../8051/8051.h"
#include "r305-fingerprint.h"

uint1 modAddress[] = {0xFF, 0xFF, 0xFF, 0xFF};
uint2 nTemp;

void cmdTransmit(uint1 *DATA, uint2 lenDATA) {
    uint1 cmd[MAX_CMD_LENGTH];
    extern uint1 modAddress[];
    uint2 checksum = 0x01, len, i;

    cmd[0] = 0xEF;
    cmd[1] = 0x01;
    for (i = 0; i < 4; i++)
        cmd[i + 2] = modAddress[i];
    cmd[6] = 0x01;

    len = lenDATA + 2;
    cmd[8] = len;
    len = len >> 8;
    cmd[7] = len;
    len = cmd[8] + (cmd[7] << 8);
    checksum += cmd[7] + cmd[8];

    for (i = 0; i < lenDATA; i++) {
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
    uint1 ack[MAX_ACK_LENGTH], DATA[lenDATA];
    uint2 i;

    Receive(ack, 11 + lenDATA);
    for (i = 0; i < lenDATA; i++) {
        DATA[i] = ack[9 + i];
    }
    
    return DATA;
}

uint1 getNTemp() {
    uint1 cmdData[] = {0x1D, '\0'};
    extern uint2 nTemp;
    uint1 *ackData, nTempL, nTempH;

    cmdTransmit(cmdData);
    ackData = cmdReceive(3);
    switch (*ackData) {
    case 0x00:
        nTempH = *(ackData + 1);
        nTempL = *(ackData + 2);
        nTemp = (nTempH << 8) + nTempL;
        return CMD_SUCCESS;
    case 0x01:
        return ERR_COMM;
    default:
        return CMD_FAIL;
    }
}

uint1 clrLib() {
    uint1 cmdData[] = {0x0D, '\0'};

    cmdTransmit(cmdData);
    switch (*cmdReceive(1)) {
    case 0x00:
        return getNTemp();
    case 0x01:
        return ERR_COMM;
    case 0x11:
        return ERR_SYS;
    default:
        return CMD_FAIL;
    }
}
