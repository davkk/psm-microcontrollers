#include <avr/io.h>
#include "twi.h"

void twi_init(void) {
    // 100 kHz
    TWBR = 72;

    twi_start(); // send START
    twi_write(0xA2); // send R/W (adres zapisu 0xA2, adres odczytu 0xA3)
    twi_write(0x0D); // CLKOUT
    twi_write(0b10000011); // CLKOUT_control (1 Hz)
    twi_stop(); // send STOP
}

void twi_get_time(uint8_t *second,
                  uint8_t *minute,
                  uint8_t *hour,
                  uint8_t *day,
                  uint8_t *week,
                  uint8_t *month,
                  uint8_t *year) {
    twi_start();
    twi_write(0xA2); // adres do zapisu
    twi_write(0x02); // start from seconds

    twi_start();
    twi_write(0xA3); // adres do odczytu

    *second = bcd_to_dec(twi_read(1) & 0b01111111);
    *minute = bcd_to_dec(twi_read(1) & 0b01111111);
    *hour = bcd_to_dec(twi_read(1) & 0b00111111);
    *day = bcd_to_dec(twi_read(1) & 0b00111111);
    *week = bcd_to_dec(twi_read(1) & 0b00000111);
    *month = bcd_to_dec(twi_read(1) & 0b00011111);
    *year = bcd_to_dec(twi_read(0) & 0b11111111);

    twi_stop();
}

void twi_set_time(uint8_t second,
                  uint8_t minute,
                  uint8_t hour,
                  uint8_t day,
                  uint8_t week,
                  uint8_t month,
                  uint8_t year) {
    twi_start();
    twi_write(0xA2);
    twi_write(2);

    twi_write(dec_to_bcd(second));
    twi_write(dec_to_bcd(minute));
    twi_write(dec_to_bcd(hour));
    twi_write(dec_to_bcd(day));
    twi_write(dec_to_bcd(week));
    twi_write(dec_to_bcd(month));
    twi_write(dec_to_bcd(year));

    twi_stop();
}

uint8_t bcd_to_dec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t dec_to_bcd(uint8_t dec) {
    return (dec / 10) << 4 | (dec % 10);
}

// procedura transmisji sygnału START
void twi_start(void) {
    // Wyzerowanie flagi TWINT ,
    // ustawienie bitu start oraz włączenie TWI
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

    // Czekamy na zakończenie operacji na magistrali I2C
    // sprawdzając flagę TWINT
    while (!(TWCR & (1<<TWINT)));
}

// procedura transmisji sygnału STOP
void twi_stop(void) {
    // Wyzerowanie flagi TWINT ,
    // ustawienie bitu stop oraz włączenie TWI
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

    // Czekamy aż zakończy się wysyłanie bitu stop
    while ((TWCR & (1<<TWSTO)));
}

// procedura transmisji bajtu
void twi_write(uint8_t byte) {
    TWDR = byte;

    // Wyzerowanie flagi TWINT i włączenie TWI
    TWCR = (1<<TWINT) | (1<<TWEN);

    // Czekamy aż zakończy się operacja
    while (!(TWCR & (1<<TWINT)));
}

// procedura odczytu bajtu danych
uint8_t twi_read(uint8_t ack) {
    // Wyzerowanie flagi TWINT i włączenie TWI
    // Wybór czy chcemy potwierdzić odczytanie danych (ack)
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA);

    // Czekamy aż zakończy się operacja
    while (!(TWCR & (1<<TWINT)));

    return TWDR; // Zwrócenie odczytanego bajtu danych
}

