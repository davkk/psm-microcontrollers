#ifndef _OW_H_
#define _OW_H_

#include <avr/io.h>
#include <util/delay.h>

#define OW_PIN PB3
#define OW_DIR DDRB
#define OW_OUT PORTB
#define OW_IN PINB

#define OW_low OW_DIR |= (1<<OW_PIN)
#define OW_high OW_DIR &= ~(1<<OW_PIN)

#define OW_check (OW_IN & (1<<OW_PIN))

uint8_t OWTouchReset(void);
void OWWriteBit(uint8_t bit);
uint8_t OWReadBit(void);

void OWWriteByte(uint8_t data);
uint8_t OWReadByte(void);

#endif
