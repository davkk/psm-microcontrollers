#include <avr/io.h>
#include "longdelay.h"

int main(void) {
    // -- silnik
    DDRC = 0xFF; // rejestr D na wyjście bo chcemy czerpać z PIND
    PORTC = ~0b11100010; // pullup resistors

    // -- buttons
    DDRD = 0x00; // rejestr D na wejście bo chcemy wpłynać na PIND
    PORTD = 0x0F; // pullup resistors

    uint16_t delay = 1;
    uint16_t direction = 0;  // 0: left, 1: right

    while (1) {
        // check if 0th button is pressed
        if (~PIND & (1<<0)) {
            delay += 1;

            // wait until released
            while (~PIND & (1<<0)) {}
        }

        // check if 0th button is pressed
        else if (~PIND & (1<<1)) {
            direction = ~direction;

            // wait until released
            while (~PIND & (1<<1)) {}
        }

        //PORTC = 0x00;
        //longdelay(1000);
        //PORTC = 0xFF;
        //longdelay(1000);

        // silnik krokowy
        for (int i = 0; i < 4; i++) {
            if (direction)
                // move left
                PORTC = 1<<i;
            else
                // move right
                PORTC = 0b00001000>>i;

            longdelay(4 * ((delay % 3) + 1));
        }

    }

    return 0;
}
