#include "../8051/8051.h"
#include "r305-fingerprint.h"

uint1 scan() {
    uint1 i;

    i = genImg();
    if (i != CMD_SUCCESS)
        return i;
    i = genChar(0x01);
    if (i != CMD_SUCCESS)
        return i;

    i = genImg();
    if (i != CMD_SUCCESS)
        return i;
    i = genChar(0x02);
    if (i != CMD_SUCCESS)
        return i;
    
    i = genTemp();
    if (i != CMD_SUCCESS)
        return i;

    return CMD_SUCCESS;
}

uint1 auth(uint2 *uID) {
    uint1 i;

    i = scan();
    if (i != CMD_SUCCESS)
        return i;

    return srchLib(0x01, uID);
}

uint1 enrol() {
    uint1 i;
    uint2 uID;

    i = scan();
    if (i != CMD_SUCCESS)
        return i;

    switch (i = srchLib(0x01, &uID)) {
    case MATCH_SUCCESS:
        return ENROL_SUCCESS;
    case ERR_COMM:
    case CMD_FAIL:
        return i;
    case MATCH_FAIL:
        i = getNTemp();
        if (i != CMD_SUCCESS)
            return i;
        return strTemp(0x01);
    }
}
