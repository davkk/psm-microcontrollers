#ifndef _USART_H
#define _USART_H

#include <avr/io.h>

void USART_init(unsigned int ubrr_value);
void USART_transmit(unsigned char data);
unsigned char USART_receive(void);

#endif
