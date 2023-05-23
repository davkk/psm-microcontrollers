#include "longdelay.h"

void longdelay(uint16_t duration) {
    for (int i = 0; i < duration; i++)
        _delay_ms(1);
}

