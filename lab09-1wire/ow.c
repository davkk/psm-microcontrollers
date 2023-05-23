#include "ow.h"

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
// (NOTE: Does not handle alarm presence from DS2404/DS1994)
//
uint8_t OWTouchReset(void) {
    uint8_t result;

    _delay_us(0);
    OW_low; // Drives DQ low
    _delay_us(480);
    OW_high; // Releases the bus
    _delay_us(70);
    result = OW_check; // Sample for presence pulse from slave
    _delay_us(410); // Complete the reset sequence recovery

    return result; // Return sample presence pulse result
}

//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void OWWriteBit(uint8_t bit) {
    if (bit) {
        // Write '1' bit
        OW_low; // Drives DQ low
        _delay_us(6);
        OW_high; // Releases the bus
        _delay_us(64); // Complete the time slot and 10us recovery

    } else {
        // Write '0' bit
        OW_low; // Drives DQ low
        _delay_us(60);
        OW_high; // Releases the bus
        _delay_us(10);
    }
}

//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
uint8_t OWReadBit(void) {
    uint8_t result;

    OW_low; // Drives DQ low
    _delay_us(6);
    OW_high; // Releases the bus
    _delay_us(9);
    result = OW_check; // Sample the bit value from the slave
    _delay_us(55); // Complete the time slot and 10us recovery

    return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void OWWriteByte(uint8_t data) {
    uint8_t loop;

    // Loop to write each bit in the byte, LS-bit first
    for (loop = 0; loop < 8; loop++) {
        OWWriteBit(data & 0x01);
        // shift the data byte for the next bit
        data >>= 1;
    }
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
uint8_t OWReadByte(void) {
    uint8_t loop, result = 0;

    for (loop = 0; loop < 8; loop++) {
        // shift the result to get it ready for the next bit
        result >>= 1;
        // if result is one, then set MS bit
        if (OWReadBit())
            result |= 0x80;
    }

    return result;
}
