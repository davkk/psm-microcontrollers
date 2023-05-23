#ifndef _TWI_H
#define _TWI_H

void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_write(uint8_t byte); 
uint8_t twi_read(uint8_t ack);

uint8_t bcd_to_dec(uint8_t bcd);
uint8_t dec_to_bcd(uint8_t dec);
void twi_get_time(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *day, uint8_t *week, uint8_t *month, uint8_t *year); 

#endif
