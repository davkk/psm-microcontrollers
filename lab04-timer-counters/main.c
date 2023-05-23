#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t time = 0;
uint8_t digit = 0;

uint8_t digits[10] = {
    //hgfedcba
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10011000  // 9
};

ISR(TIMER1_COMPA_vect) {
    time++;
}

ISR(TIMER0_COMP_vect) {
    digit = (digit + 1) % 4;

    PORTD = ~(1<<digit);

    if (digit == 0) PORTC = digits[time % 10];
    if (digit == 1) PORTC = digits[(time / 10) % 10];
    if (digit == 2) PORTC = digits[(time / 100) % 10];
    if (digit == 3) PORTC = digits[(time / 1000) % 10];
}

int main(void) {
    // konfiguracja kabelkow, wszystkie wyjsciowe
    DDRC = 0xFF;
    DDRD = 0xFF;

    // -- PIERWSZY TIMER
    // 16_000_000 / 1024 / 1Hz = 15625 (16bit)

    // ustawienie timer/counter1
    // tryb porownawczy (CTC) z rejestrem OCR1A
    TCCR1B |= (1<<WGM12);

    // prescaler 1024
    TCCR1B |= (1<<CS10) | (1<<CS12);

    // ustawienie wartosci w rejestrze do porownania
    // do tej wartosci licznik bedzie liczyl
    OCR1A = 16;

    // wlaczenie przerwania przy porownaniu licznika z OCR1A
    TIMSK |= (1<<OCIE1A);

    // -- DRUGI TIMER
    // 16_000_000 / 1024 / 200Hz = 78.??? (8bit)
    // 16_000_000 / 64 / 1000Hz = 250 (8bit) => lepiej

    // ustawienie timer/counter0
    // tryb CTC z OCR0
    TCCR0 |= (1<<WGM01);

    // prescaler 1024
    TCCR0 |= (1<<CS00) | (1<<CS01);

    // Output Compare Register OCR0
    OCR0 = 250;

    // uruchomienie przerwania przy porownaniu timer/counter0 z OCR0
    TIMSK |= (1<<OCIE0);

    sei();

    while (1) {}
    return 0;
}
