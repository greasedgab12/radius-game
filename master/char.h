#ifndef CHARS_H
#define CHARS_H
#include <avr/pgmspace.h>
#include <inttypes.h>

//contains all chars
extern const uint8_t alphabet[46][14] PROGMEM;

void sendChar(uint8_t x,uint8_t y,uint8_t lx,uint8_t ly, const uint8_t Bstabe);


void print(const char *text,uint8_t x,uint8_t y);
void printN(const uint16_t *number,uint8_t x,uint8_t y);
void printB(const uint16_t *number,uint8_t x,uint8_t y);
void print32(const uint32_t number,uint8_t x,uint8_t y);

#endif
