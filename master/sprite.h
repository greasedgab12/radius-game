#ifndef SPRITE_H
#define SPRITE_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define MAX_SPRITE_COUNT 16

//array of pointers to sprite data
uint8_t* sprite_in_ram[MAX_SPRITE_COUNT];

uint8_t* load_sprite(uint8_t* sprite);
void flush_sprite(uint8_t *sprite);

extern const uint8_t enemy_1[29] PROGMEM;
extern const uint8_t vessel_0[18] PROGMEM;
extern const uint8_t dino[198] PROGMEM;
extern const uint8_t player_4[29] PROGMEM;
extern const uint8_t radius[202] PROGMEM;
extern const uint8_t player_2[29] PROGMEM;
extern const uint8_t enemy_2[29] PROGMEM;
extern const uint8_t player_3[29] PROGMEM;
extern const uint8_t projectile_0[7] PROGMEM;
extern const uint8_t enemy_0[29] PROGMEM;
extern const uint8_t shade[82] PROGMEM;
extern const uint8_t projectile_1[6] PROGMEM;
extern const uint8_t projectile_2[14] PROGMEM;
extern const uint8_t enemy_4[29] PROGMEM;
extern const uint8_t enemy_3[29] PROGMEM;
extern const uint8_t player_1[29] PROGMEM;
extern const uint8_t player_0[29] PROGMEM;
extern const uint8_t projectile_3[10] PROGMEM;

#endif
