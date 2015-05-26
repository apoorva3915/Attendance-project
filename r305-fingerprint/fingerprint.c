#include "../8051/8051.h"
#include "r305-fingerprint.h"

uint1 genImg() {
    uint1 cmdData[] = {0x01, '\0'};

    cmdTransmit(cmdData);
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
    uint1 cmdData[] = {0x02, buffID, '\0'};

    cmdTransmit(cmdData);
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
    uint1 cmdData[] = {0x05, '\0'};

    cmdTransmit(cmdData);
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
