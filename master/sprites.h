#ifndef SPRITES_H
#define SPRITES_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define BULLETSHORT_SPRITE 0
extern const uint8_t bulletShort_sprite[6] PROGMEM;
#define PLAYER_0 1
extern const uint8_t player_0[29] PROGMEM;
#define DISC_SPRITE 2
extern const uint8_t disc_sprite[14] PROGMEM;
#define ENEMY_2 3
extern const uint8_t enemy_2[29] PROGMEM;
#define BALL_SPRITE 4
extern const uint8_t ball_sprite[18] PROGMEM;
#define PLAYER_4 5
extern const uint8_t player_4[29] PROGMEM;
#define SHOTMIDDLE 6
extern const uint8_t shotmiddle[6] PROGMEM;
#define SHADE 7
extern const uint8_t shade[82] PROGMEM;
#define SHOTLOWER 8
extern const uint8_t shotlower[6] PROGMEM;
#define ENEMY_4 9
extern const uint8_t enemy_4[29] PROGMEM;
#define DINO 10
extern const uint8_t dino[198] PROGMEM;
#define ENEMY_3 11
extern const uint8_t enemy_3[29] PROGMEM;
#define MISSILE_SPRITE 12
extern const uint8_t missile_sprite[10] PROGMEM;
#define RADIUS 13
extern const uint8_t radius[202] PROGMEM;
#define PLAYER_3 14
extern const uint8_t player_3[29] PROGMEM;
#define ENEMY_1 15
extern const uint8_t enemy_1[29] PROGMEM;
#define PLAYER_1 16
extern const uint8_t player_1[29] PROGMEM;
#define ENEMY_0 17
extern const uint8_t enemy_0[29] PROGMEM;
#define BULLETHEAVY_SPRITE 18
extern const uint8_t bulletHeavy_sprite[14] PROGMEM;
#define SHOTUPPER 19
extern const uint8_t shotupper[6] PROGMEM;
#define PLAYER_2 20
extern const uint8_t player_2[29] PROGMEM;
#define BULLET_SPRITE 21
extern const uint8_t bullet_sprite[7] PROGMEM;
#define VESSEL_0 22
extern const uint8_t vessel_0[18] PROGMEM;

uint8_t getSpriteIndex(uint8_t* sprite);
#endif