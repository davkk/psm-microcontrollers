#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// f = 1 / 20ms = 50hz -> wymagane do sterowania serwo sygnałem
// Fast PWM - mode 14
//
// sprawdzam N = 1024 (prescaler)
// 16_000_000MHz / 1024 = 15_625MHz -> 64us period
// -- (wzór str. 101):
// -- ICR1 = 16_000_000 / (50 * N) - 1 = 311.5
// -- 1 / 20 = OCR1A / ICR1(TOP)
// czyli wychodzi OCR1A in [16, 2*16]
//
// jednak N = 8 (prescaler)
// 16_000_000MHz / 8 = 2_000_000MHz -> 0.5us period
// -- ICR1 = 16_000_000 / (50 * 8) - 1 = 39999
// -- 1 / 20 = OCR1A / ICR1(TOP)
// -- OCR1A = 39999 / 20 = 1999.95
// zatem OCR1A in [2000, 2*2000]
// => lepiej

// -- USART FUNCTIONS

void USART_Init(unsigned int ubrr_value) {
    UBRRH = (unsigned char)(ubrr_value>>8);
    UBRRL = (unsigned char)ubrr_value;

    UCSRC = (1<<URSEL) | (1<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);

    UCSRB = (1<<RXEN) | (1<<TXEN);

    UCSRB |= (1<<RXCIE);
}

void USART_Transmit(unsigned char data) {
    while (~UCSRA & (1<<UDRE)) {}
    UDR = data;
}

unsigned char USART_Recieve(void) {
    while (~UCSRA & (1<<RXC)) {}
    return UDR;
}

// -- INTERRUPTS

char buffor[10];
uint8_t i = 0;

ISR(USART_RXC_vect) { // SIG_USART_RECV
    buffor[i] = UDR;

    if (buffor[i] == '\n') {
        OCR1A = atoi((const char*)buffor);

        i = 0;
    }
    else {
        i++;
    }
}


// -- MAIN

int main(void) {
    // wyjsciowe
    DDRD = 0xFF;
    PORTD |= (1<<PD5); // PD5 == OCR1A

    // ustawienie timer/counter1
    // compare output mode -> non inverting mode
    TCCR1A |= (1<<COM1A1);

    // tryb Fast PWM {
    TCCR1A |= (1<<WGM11);
    TCCR1B |= (1<<WGM12) | (1<<WGM13);
    // }

    // prescaler N = 8
    TCCR1B |= (1<<CS11);

    // part1: servomechanism
    // (the shaft rotates to a specific angle depending on the duty cycle of the pulse)
    //ICR1 = 39999;

    // part2: diode
    // timer/counter1: count from 0 to 100
    ICR1 = 100;

    USART_Init(3); // 3 = 250kbps (predkosc transmisji/baudrate)

    sei();

    uint8_t x = 0;
    int8_t d = 1;

    while (1) {
        // part2: diode
        if (x == 100) d = -1;
        else if (x == 0) d = 1;

        x += d;
        OCR1A = x;

        _delay_ms(20);
    }

    return 0;
}
