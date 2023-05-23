#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "usart.h"

// -- OUTPUT STREAM

char result;

int u_putchar(char c, FILE *stream) {
    USART_transmit(c);
    return 0;
}

static FILE u_out =
    FDEV_SETUP_STREAM(u_putchar, NULL, _FDEV_SETUP_WRITE);


// -- INTERRUPTS

char buffor[10];
uint8_t i = 0;
uint8_t x, y;

ISR(USART_RXC_vect) { // SIG_USART_RECV
    buffor[i] = UDR; // read off received character

    if (buffor[i] == '\n') {
        //// command "on"
        //if (strncmp((const char*)buffor, "on", 2) == 0)
        //    PORTA &= ~(1<<0);

        //// command "off"
        //if (strncmp((const char*)buffor, "off", 3) == 0)
        //    PORTA |= (1<<0);

        if (buffor[0] == '+')
            fprintf(&u_out, "%d + %d = %d\n", x, y, x + y);
        else if (buffor[0] == '-')
            fprintf(&u_out, "%d - %d = %d\n", x, y, x - y);
        else if (buffor[0] == '*')
            fprintf(&u_out, "%d * %d = %d\n", x, y, x * y);
        else if (buffor[0] == '/') {
            if (y == 0)
                fprintf(&u_out, "undefined!\n");
            else
                fprintf(&u_out, "%d / %d = %d\n", x, y, x / y);
        }
        else {
            x = y;
            y = atoi((const char*)buffor);
        }

        i = 0;
    }
    else {
        i++;
    }
}

int main(void) {
    DDRA = 0xFF;
    PORTA = 0x00;

    USART_init(3); // 3 = 250kbps

    sei();

    while (1) {
       // result = USART_recieve();
       // if (result == 'a')
       //     fprintf(&u_out, "hello, world!");
    }

    return 0;
}
