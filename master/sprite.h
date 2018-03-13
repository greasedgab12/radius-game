#ifndef SPRITE_H
#define SPRITE_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define MAX_SPRITE_COUNT 32

//array of pointers to sprite data
extern uint8_t* sprite_in_ram[MAX_SPRITE_COUNT];

uint8_t* load_sprite(const uint8_t* sprite);
void flush_sprite(const uint8_t* sprite);

void flushAllSprites();

void drawTitleScreen();

void spriteInit();

#endif
