#include "../8051/8051.h"
#include "r305-fingerprint.h"

/* Each command/data package contains the module address. FP responds to 
   package whose address item is same as its identifying address. 
   In the current state, there is no function defined to modify the module 
   address. */
uint1 modAddress[] = {0xFF, 0xFF, 0xFF, 0xFF};

/* Sends a command packet to FP. First argument is pointer to the first byte 
   of the DATA item to be sent in the command packet, and second argument is 
   the length of the DATA item (in bytes). Leading and trailing metadata - 
   module address, length, checksum, etc - are all prepended/appended. */
void cmdTransmit(uint1 *DATA, uint2 lenDATA) {
    uint1 cmd[lenDATA + 11];
    // Leading and trailing metadata - length, checksum, etc - is 11 bytes.
    extern uint1 modAddress[];
    uint2 checksum = 0x01, len, i;
    // Package identifier of a command packet is 0x01.

    cmd[0] = 0xEF; cmd[1] = 0x01;
    // Prepend the module address.
    for (i = 0; i < 4; i++)
        cmd[i + 2] = modAddress[i];
    // Prepend the packagge identifier.
    cmd[6] = 0x01;
    // Checksum is 2 bytes long.
    len = lenDATA + 2;
    cmd[8] = len;
    len = len >> 8;
    cmd[7] = len;
    len = cmd[8] + (cmd[7] << 8);
    // Add bytes of len to checksum.
    checksum += cmd[7] + cmd[8];
    for (i = 0; i < lenDATA; i++) {
        cmd[i + 9] = *(DATA + i);
        checksum += *(DATA + i);
    }
    i += 9;
    // Append the checksum.
    cmd[++i] = checksum;
    checksum = checksum >> 8;
    cmd[--i] = checksum;

    Transmit(cmd, 9 + len);
}

/* Returns a pointer to the first byte of DATA item in the acknowledgement 
   packet received from FP. Leading and trailing metadata is stripped. The 
   length of the DATA item (in bytes) must be supplied. 
   In the current state, the function does not verify the checksum nor the 
   module address, and is uncapable of receving any data packet (in case FP 
   is requested to upload image/template). */
uint1 *cmdReceive(uint2 lenDATA) {
    uint1 ack[lenDATA + 11], DATA[lenDATA];
    // Leading and trailing metadata - length, checksum, etc - is 11 bytes.
    uint2 i;

    Receive(ack, lenDATA + 11);
    for (i = 0; i < lenDATA; i++) {
        DATA[i] = ack[9 + i];
    }
    return DATA;
}

/* Get the number of templates stored in the flash library of FP and store it 
   at the address supplied as argument. */
uint1 getNTemp(uint2 *nTemp) {
    uint1 cmdData[] = {0x1D};
    uint1 *ackData;

    cmdTransmit(cmdData, 1);
    ackData = cmdReceive(3);
    switch (*ackData) {
    case 0x00:
        *nTemp = (*(ackData + 1) << 8) + *(ackData + 2);
        return CMD_SUCCESS;
    case 0x01:
        return ERR_COMM;
    default:
        return CMD_FAIL;
    }
}

/* Clear the flash library of FP, erasing all the stored templates. */
uint1 clrLib(void) {
    uint1 cmdData[] = {0x0D}, i;
    uint2 nTemp;

    cmdTransmit(cmdData, 1);
    switch (*cmdReceive(1)) {
    case 0x00:
        if ((i = getNTemp(&nTemp)) != CMD_SUCCESS)
            return i;
        return (!nTemp) ? CMD_SUCCESS : CMD_FAIL;
    case 0x01:
        return ERR_COMM;
    case 0x11:
        return ERR_SYS;
    default:
        return CMD_FAIL;
    }
}
