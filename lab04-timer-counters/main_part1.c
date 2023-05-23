#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect) {
    if (~PIND & (1<<6)) PORTC ^= (1<<0); // toggle diode
}

int main(void) {
    // -- button
    DDRD &= ~(1<<6); // konfiguracja przycisku jako input (domyślnie)
    PORTD |= (1<<6); // podpiecie rezystora podciagajacego

    // -- led
    DDRC |= (1<<0); // nozka wyjsciowa dla diody
    PORTC |= (1<<0); // ustawienie zeby dioda sie domyslnie nie swiecila

    // ustawienie pierwszego licznika w porownawczy tryb pracy
    // (CTC, porownanie z rejestrem OCR1A)
    TCCR1B |= (1<<WGM12);

    // ustawienie maksymalnego prescalera 1024
    TCCR1B |= (1<<CS10) | (1<<CS12);

    // ustawienie wartosci w rejestrze do porownania
    // wzór: str. 99
    // oczekiwana czestotliwosc: f = 1 Hz
    OCR1A = 7812; // (do tej wartosci licznik bedzie liczyl)

    // the Timer/Counter1 Output Compare A match interrupt is enabled
    // The corresponding Interrupt Vector is executed when the OCF1A Flag
    // (wlaczenie przerwania przy porownaniu licznika z OCR1A)
    TIMSK |= (1<<OCIE1A); // timer iterrupt mask (TIMSK)

    sei();

    while (1) {}
    return 0;
}
