#include <avr/interrupt.h>
#include <stdio.h>

#include "lcd.h"
#include "twi.h"

static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

uint8_t second, minute, hour, day, week, month, year;
char* days[7] = {"pn", "wt", "sr", "cz", "pt", "sb", "nd"};

ISR(INT0_vect) {
    twi_get_time(&second, &minute, &hour, &day, &week, &month, &year);

    lcd_set_xy(0, 0);
    fprintf(&lcd_out, "%02d:%02d:%02d", hour, minute, second);

    lcd_set_xy(14, 0);
    fprintf(&lcd_out, "%s", days[week]);

    lcd_set_xy(0, 1);
    fprintf(&lcd_out, "%02d/%02d/20%02d", day, month, year);
}

int main(void) {
    lcd_init();
    lcd_write_instr(0x0C); // ukrycie kursora

    twi_init();
    twi_set_time(56, 59, 23, 31, 6, 12, 23);

    MCUCR |= (1<<ISC00); // interrupt 0 to falling edge
    GICR |= (1<<INT0);

    // wlaczenie przerwan
    sei();

    while (1) {}

    return 0;
}
