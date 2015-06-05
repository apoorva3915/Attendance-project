#include "../8051/8051.h"
#include "r305-fingerprint.h"

/* Scan the fingerprint and generate the its template, which is stored in both 
   the character buffers. */
uint1 scan(void) {
    extern void Delaym(unsigned int time);
    uint1 i;

    // Scan the fingerprint and store character file in buff01.
    i = genImg();
    if (i != CMD_SUCCESS)
        return i;
    i = genChar(0x01);
    if (i != CMD_SUCCESS)
        return i;
    Delaym(100);
    // Scan the fingerprint again and store character file in buff02.
    i = genImg();
    if (i != CMD_SUCCESS)
        return i;
    i = genChar(0x02);
    if (i != CMD_SUCCESS)
        return i;
    // Generate template from character files in the buffers and store the 
    // generated character file back in both the buffers.
    i = genTemp();
    if (i != CMD_SUCCESS)
        return i;

    return CMD_SUCCESS;
}

/* Authenticate the template currently stored in the buffers against those 
   stored in the flash library of FP. Each template stored in the flash 
   library has a unique ID associated with it. The first argument is the 
   address at which to store this unique ID of the template, if authenticated 
   successfully. */
uint1 auth(uint2 *uID) {
    extern uint1 srchLib(uint1 buffID, uint2 *pageID);
    // Search library for matching template in buff01.
    return srchLib(0x01, uID);
}

/* Store the template (currently in the buffers) in the flash library of FP. */
uint1 enrol(void) {
    extern uint1 strTemp(uint1 buffID);
    extern uint1 srchLib(uint1 buffID, uint2 *pageID);
    uint1 i;
    uint2 uID;

    // Search the library if the template already exists.
    switch (i = srchLib(0x01, &uID)) {
    case MATCH_SUCCESS:
        // Already exists so no need to store again.
        return ENROL_SUCCESS;
    case MATCH_FAIL:
        // Does not exist in library, so store it.
        return strTemp(0x01);
    default:
        return i;
    }
}
