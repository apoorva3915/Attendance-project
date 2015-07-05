#include "../8051/8051.h"
#include "r305-fingerprint.h"

unsigned char genImg() {
    unsigned char cmdData[] = {0x01};

    cmdTransmit(cmdData, 1);
    switch (*cmdReceive(1)) {
    case 0x00:
        return CMD_SUCCESS;
    case 0x01:
        return ERR_COMM;
    case 0x02:
    case 0x03:
        return ERR_SCAN;
    default:
        return CMD_FAIL;
    }
}

unsigned char genChar(unsigned char buffID) {
    unsigned char cmdData[2];
    cmdData[0] = 0x02;
    cmdData[1] = buffID;

    cmdTransmit(cmdData, 2);
    switch (*cmdReceive(1)) {
    case 0x00:
        return CMD_SUCCESS;
    case 0x01:
        return ERR_COMM;
    case 0x06:
    case 0x07:
    case 0x15:
        return ERR_SCAN;
    default:
        return CMD_FAIL;
    }
}

unsigned char genTemp() {
    unsigned char cmdData[] = {0x05};

    cmdTransmit(cmdData, 1);
    switch (*cmdReceive(1)) {
    case 0x00:
        return CMD_SUCCESS;
    case 0x01:
        return ERR_COMM;
    case 0x0A:
        return ERR_TEMP;
    default:
        return CMD_FAIL;
    }
}

unsigned char strTemp(unsigned char buffID) {
    unsigned char pageIDL, pageIDH, cmdData[4];
    extern unsigned int nTemp;
    ++nTemp;
    pageIDL = nTemp;
    pageIDH = nTemp >> 8;
    cmdData[0] = 0x06;
    cmdData[1] = buffID;
    cmdData[2] = pageIDH;
    cmdData[3] = pageIDL;

    cmdTransmit(cmdData, 4);
    switch (*cmdReceive(1)) {
    case 0x00:
        return ENROL_SUCCESS;
    case 0x01:
        --nTemp;
        return ERR_COMM;
    case 0x0B:
    case 0x18:
        --nTemp;
        return ENROL_FAIL;
    default:
        --nTemp;
        return CMD_FAIL;
    }
}

unsigned char srchLib(unsigned char buffID, unsigned int *pageID) {
    unsigned char cmdData[6], *ackData, pageIDH, pageIDL;
    cmdData[0] = 0x04;
    cmdData[1] = buffID;
    cmdData[2] = 0x00;
    cmdData[3] = 0x00;
    cmdData[4] = 0x01;
    cmdData[5] = 0xFF;
    
    cmdTransmit(cmdData, 6);
    ackData = cmdReceive(5);
    switch (*ackData) {
    case 0x00:
        pageIDH = *(ackData + 1);
        pageIDL = *(ackData + 2);
        *pageID = (pageIDH << 8) + pageIDL;
        return MATCH_SUCCESS;
    case 0x01:
        return ERR_COMM;
    case 0x09:
        return MATCH_FAIL;
    default:
        return CMD_FAIL;
    }
}
