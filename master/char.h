#ifndef CHAR_H
#define CHAR_H
#include <avr/pgmspace.h>
#include <inttypes.h>

//contains all chars
extern const uint8_t alphabet[42][14] PROGMEM;

void print(const char *text,uint8_t x,uint8_t y);
void printN(const uint16_t *number,uint8_t x,uint8_t y);
void printB(const uint8_t *number,uint8_t x,uint8_t y);



#endif
