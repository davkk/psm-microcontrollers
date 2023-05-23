#include <avr/interrupt.h>
#include <stdio.h>

#include "lcd.h"
#include "ow.h"

static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void) {
    lcd_init();
    lcd_write_instr(0x0C); // ukrycie kursora

    while (1) {
		OWTouchReset(); // we don't care about success/fail return value
		OWWriteByte(0xCC); // SKIPROM
		OWWriteByte(0x44); // init temperature conversion

		_delay_ms(750);
		
		OWTouchReset(); // we need to greet again
		OWWriteByte(0xCC); // SKIPROM
		OWWriteByte(0xBE); // start transmit data to master

		uint8_t LSByte = OWReadByte();
		uint8_t MSByte = OWReadByte();
		uint16_t result = (MSByte << 8) | LSByte; // 0b<MSByte><LSByte>

		float temperature = 0.0625 * result;

		lcd_set_xy(0, 0);
		fprintf(&lcd_out, "T = %.4f %cC", temperature, 0b11011111); // 0b11011111 = 223
	}

    return 0;
}
