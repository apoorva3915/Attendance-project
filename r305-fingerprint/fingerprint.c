#include "../8051/8051.h"
#include "r305-fingerprint.h"

uint1 genImg() {
    uint1 cmdData[] = {0x01};

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

uint1 genChar(uint1 buffID) {
    uint1 cmdData[] = {0x02, buffID};

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

uint1 genTemp() {
    uint1 cmdData[] = {0x05};

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

uint1 strTemp(uint1 buffID) {
    uint1 pageIDL, pageIDH;
    extern uint2 nTemp;
    ++nTemp;
    pageIDL = nTemp;
    pageIDH = nTemp >> 8;
    uint1 cmdData[] = {0x06, buffID, pageIDH, pageIDL};

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

uint1 srchLib(uint1 buffID, uint2 *pageID) {
    uint1 cmdData[] = {0x04, buffID, 0x00, 0x00, 0x01, 0xFF};
    uint1 *ackData, pageIDH, pageIDL;

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
