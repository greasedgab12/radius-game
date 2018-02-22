#ifndef CHARS_H
#define CHARS_H
#include <avr/pgmspace.h>
#include <inttypes.h>

//contains all chars
extern const uint8_t alphabet[36][14] PROGMEM;

void print(const char *text,uint8_t x,uint8_t y);

#endif
