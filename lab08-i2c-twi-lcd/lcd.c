#include "lcd.h"

void lcd_write_instr(uint8_t data) {
    LCD_PORT &= ~(1<<LCD_RS); // zerowanie RS, wysylanie instrukcji a nie danej

    LCD_PORT |= 1<<LCD_E; // enable przesyl danych
    LCD_PORT = (data & 0xF0) | (LCD_PORT & 0x0F);
    _delay_ms(4); // opoznienie 4 ms
    LCD_PORT &= ~(1<<LCD_E); // zamkniecie przesylu danych

    _delay_ms(4);

    LCD_PORT |= 1<<LCD_E; // enable przesyl danych
    LCD_PORT = (data<<4) | (LCD_PORT & 0x0F);
    _delay_ms(4);
    LCD_PORT &= ~(1<<LCD_E); // zamkniecie przesylu danych

    _delay_ms(4);
}

void lcd_write_data(uint8_t data) {
    LCD_PORT |= 1<<LCD_RS; // wysylanie danej a nie instrukcji

    LCD_PORT |= 1<<LCD_E; // enable przesyl danych
    LCD_PORT = (data & 0xF0) | (LCD_PORT & 0x0F);
    _delay_ms(4); // opoznienie 4 ms
    LCD_PORT &= ~(1<<LCD_E); // zamkniecie przesylu danych

    _delay_ms(4);

    LCD_PORT |= 1<<LCD_E; // enable przesyl danych
    LCD_PORT = (data<<4) | (LCD_PORT & 0x0F);
    _delay_ms(4);
    LCD_PORT &= ~(1<<LCD_E); // zamkniecie przesylu danych
}

void lcd_init(void) {
    _delay_ms(45);

    LCD_DDR = 0xFF; // caly port jako output

    lcd_write_instr(0x33); // najpierw wysle 3 potem 3
    lcd_write_instr(0x32); // najpierw wysle 3 potem 2

    lcd_write_instr(0x28); 
    lcd_write_instr(0x08); 
    lcd_write_instr(0x01); 
    lcd_write_instr(0x06); 
    lcd_write_instr(0x0F); // ustaw tryb wyswietlacza
}

void lcd_clear(void) {
    lcd_write_instr(0x01);
}

void lcd_set_xy(uint8_t x, uint8_t y) {
    lcd_write_instr(0x80 + x + 0x40 * y);
}

void lcd_write_text_xy(uint8_t x, uint8_t y, char *text) {
    lcd_set_xy(x, y);

    while (*text) {
        lcd_write_data(*text);
        text++;
    }
}

int lcd_putchar(char c, FILE *stream) {
    lcd_write_data(c);
    return 0;
}
