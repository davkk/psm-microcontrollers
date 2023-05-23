#include "usart.h"

void USART_init(unsigned int ubrr_value) {
    UBRRH = (unsigned char)(ubrr_value>>8);
    UBRRL = (unsigned char)ubrr_value;

    UCSRC = (1<<URSEL) | (1<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);

    UCSRB = (1<<RXEN) | (1<<TXEN);

    UCSRB |= (1<<RXCIE);
}

void USART_transmit(unsigned char data) {
    while (~UCSRA & (1<<UDRE)) {}
    UDR = data;
}

unsigned char USART_receive(void) {
    while (~UCSRA & (1<<RXC)) {}
    return UDR;
}
