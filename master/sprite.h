#ifndef SPRITE_H
#define SPRITE_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define MAX_SPRITE_COUNT 16

//array of pointers to sprite data
uint8_t* sprite_in_ram[MAX_SPRITE_COUNT];

uint8_t* load_sprite(const uint8_t* sprite);
void flush_sprite(const uint8_t* sprite);

#endif
