#include <stdio.h>

#include "lcd.h"

#define MOSI PB5
#define SCK PB7
#define SS PB4

void InitSPI(void) {
	// ustawienie kierunku wyjściowego dla linii: MOSI | SCK | SS
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS);

	// aktywacja SPI | tryb Master | prędkość zegara fosc/64
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR1);
}

uint8_t TransferSPI(uint8_t byte)
{
    SPDR = byte;
    // czekamy na ustawienie flagi SPIF po
    // zakończeniu transmisji
    while (!(SPSR & (1<<SPIF)));
    return SPDR;
}

static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void) {
    InitSPI();
    lcdinit();

    // disable SS
    PORTB |= (1<<SS);

    uint8_t result1, result2;
    uint16_t d_out;

    float voltage;
    float voltages[5] = {0};
    float sum;

    while (1) {
        // enable SS
        PORTB &= ~(1<<SS);

        // start bit
        TransferSPI(1);

        // config: channel 0, single ended mode, MSB first;
        result1 = TransferSPI(0b10100000);

        // wysylamy cokolwiek zeby dostac kolejne 8 bits
        result2 = TransferSPI(0);

        // disable SS
        PORTB |= (1<<SS);

        // collect all bits
        // 0b<MSB><LSB>
        d_out = ((result1 & 0x0F) << 8) | result2;
        voltage = 5.0 * d_out / 4096;

        lcd_set_xy(0, 0);
        fprintf(&lcd_out, "V = %.2f", voltage);

        // -- moving average of voltages
        for (int i = 0; i < 4; ++i) voltages[i] = voltages[i + 1];
        voltages[4] = voltage;

        lcd_set_xy(0, 1);

        sum = 0;
        for (int i = 0; i < 5; ++i)
            sum += voltages[i];

        fprintf(&lcd_out, "V_avg = %.2f", sum / 5);
    }

    return 0;
}
