#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define LCD_PORT PORTA
#define LCD_DDR DDRA
#define LCD_E 3
#define LCD_RS 2

//inicjalizacja wyswietlacza
void lcdinit(void); 

//funkcja wysyłająca pojedynczą instrukcję do wyświetlacza↪→
void lcd_write_instr(uint8_t data); 

//funkcja realizująca instrukcję "czyść wyświetlacz"
void lcd_clear(void); 

//funkcja wysyłająca pojedynczą daną do wyświetlacza↪→
void lcd_write_data(uint8_t data); 

//funkcja ustawiająca kursor na pozycji xy↪→//funkcja ustawiająca kursor na pozycji xy↪→
void lcd_set_xy(uint8_t x, uint8_t y); 

//funkcja wyświetlająca tekst znajdujący sie w w pamięci danych(!!!) począwszy od pozycji xy.
void lcd_write_text_xy(uint8_t x, uint8_t y, char *text); 

int lcd_putchar(char c, FILE *stream);
#endif
