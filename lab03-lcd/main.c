//#include <string.h>
#include <stdio.h>

#include "lcd.h"

static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void) {
    lcdinit();

    lcd_write_instr(0b01000000);

    lcd_write_data(0b00000000);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00001010);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00010001);
    lcd_write_data(0b00001110);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00000000);

    lcd_write_data(0b00000000);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00001010);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00001110);
    lcd_write_data(0b00010001);
    lcd_write_data(0b00000000);
    lcd_write_data(0b00000000);

    lcd_set_xy(0, 0);
    lcd_write_data(0);
    lcd_set_xy(0, 1);
    lcd_write_data(1);

    //fprintf(&lcd_out, "hello, %.2f!", 3.14);

    //for (int i = 0; i < strlen(message); i++) lcd_write_data(message[i]);
    //lcd_write_text_xy(0, 0, "hello,");
    //_delay_ms(3000);
    //lcd_clear();
    //_delay_ms(1000);
    //lcd_set_xy(5, 1);
    //_delay_ms(1000);
    //lcd_write_text_xy(0, 1, "friend.");

    while (1) {}

    return 0;
}
