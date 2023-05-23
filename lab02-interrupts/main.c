#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

uint8_t counts = 1;

ISR(INT0_vect) {
    // zero to świeci, jeden to nie świeci!!
    PORTC = ~counts; // przypisanie wartosci counts do rejestru, negacja!
    counts++;

    // czekaj aż puszczę przycisk
    _delay_ms(100);
}

int main(void) {
    MCUCR |= 1<<ISC01; // zbocze opadające
    GICR |= 1<<INT0; // włącz przerwanie na INT0

    DDRC = 0xFF; // ustawienie kierunku portu C na output
    DDRD = 0x00; // nozki portu D ustawione na input (default)
    PORTD = 0xFF; // podpiecie rezystorow podciagajacych dla portu D

    sei(); // wlaczenie podzespolu przerwan

    while (1) {}

    return 0;
}
