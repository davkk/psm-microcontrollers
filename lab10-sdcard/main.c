#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "FatFs/diskio.h"
#include "FatFs/ffconf.h"

#include "lcd.h"
#include "ow.h"

static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

FATFS FatFs;
FIL fil;
FRESULT fr;
UINT br, bw;

char line[100];

int main(void) {
    DDRD = 0x00;
    PORTD = 0xFF;

    lcd_init();
    lcd_write_instr(0x0C); // ukrycie kursora

    f_mount(&FatFs, "", 0); // drugi argument: nazwa dysku, trzeci: do not mount now

    // reading from file on sd card
    //fr = f_open(&fil, "hello.txt", FA_READ);
    //fr = f_read(&fil, line, sizeof line, &br); /* Read a chunk of data from the source file */
    //fprintf(&lcd_out, "%s", line);

    // writing to file on sd
    fr = f_open(&fil, "data.txt", FA_CREATE_ALWAYS | FA_WRITE);

    while (1) {
        if (~PIND & (1<<0)) {
            f_close(&fil);
            while (1);
        } 

        OWTouchReset(); // we don't care about success/fail return value
        OWWriteByte(0xCC); // SKIPROM
        OWWriteByte(0x44); // init temperature conversion

		_delay_ms(1000);
		
		OWTouchReset(); // we need to greet again
		OWWriteByte(0xCC); // SKIPROM
		OWWriteByte(0xBE); // start transmit data to master

		uint8_t LSByte = OWReadByte();
		uint8_t MSByte = OWReadByte();
		uint16_t result = (MSByte << 8) | LSByte; // 0b<MSByte><LSByte>

		float temperature = 0.0625 * result;

		lcd_set_xy(0, 0);
		fprintf(&lcd_out, "T = %.4f %cC", temperature, 0b11011111); // 0b11011111 = 223

        sprintf(line, "%.4f\n", temperature);
        fr = f_write(&fil, line, strlen(line), &bw);
	}

    return 0;
}
