#ifndef SPRITES_H
#define SPRITES_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define BULLETSHORT_SPRITE 0
extern const uint8_t bulletShort_sprite[6] PROGMEM;
#define PLAYER_0 1
extern const uint8_t player_0[26] PROGMEM;
#define COLLISION_SPRITE 2
extern const uint8_t collision_sprite[10] PROGMEM;
#define DISC_SPRITE 3
extern const uint8_t disc_sprite[14] PROGMEM;
#define ENEMY_2 4
extern const uint8_t enemy_2[29] PROGMEM;
#define BALL_SPRITE 5
extern const uint8_t ball_sprite[18] PROGMEM;
#define ENERGYBAR_0 6
extern const uint8_t energybar_0[86] PROGMEM;
#define BULLETENEMY_SPRITE 7
extern const uint8_t bulletEnemy_sprite[6] PROGMEM;
#define SHOTMIDDLE_SPRITE 8
extern const uint8_t shotmiddle_sprite[10] PROGMEM;
#define SHADE 9
extern const uint8_t shade[82] PROGMEM;
#define SHOTUPPER_SPRITE 10
extern const uint8_t shotupper_sprite[10] PROGMEM;
#define LASER0_SPRITE 11
extern const uint8_t laser0_sprite[3] PROGMEM;
#define ENEMY_4 12
extern const uint8_t enemy_4[29] PROGMEM;
#define DINO 13
extern const uint8_t dino[198] PROGMEM;
#define ENEMY_3 14
extern const uint8_t enemy_3[29] PROGMEM;
#define MISSILE_SPRITE 15
extern const uint8_t missile_sprite[10] PROGMEM;
#define LASER2_SPRITE 16
extern const uint8_t laser2_sprite[3] PROGMEM;
#define RADIUS 17
extern const uint8_t radius[866] PROGMEM;
#define PLAYER_3 18
extern const uint8_t player_3[38] PROGMEM;
#define ENEMY_1 19
extern const uint8_t enemy_1[29] PROGMEM;
#define ENERGYBAR_1 20
extern const uint8_t energybar_1[86] PROGMEM;
#define PLAYER_1 21
extern const uint8_t player_1[20] PROGMEM;
#define ENEMY_0 22
extern const uint8_t enemy_0[29] PROGMEM;
#define BULLETHEAVY_SPRITE 23
extern const uint8_t bulletHeavy_sprite[14] PROGMEM;
#define LASER1_SPRITE 24
extern const uint8_t laser1_sprite[3] PROGMEM;
#define EXPLOSION_SPRITE 25
extern const uint8_t explosion_sprite[18] PROGMEM;
#define HEALTHBAR_1 26
extern const uint8_t healthbar_1[86] PROGMEM;
#define IMPACT_SPRITE 27
extern const uint8_t impact_sprite[6] PROGMEM;
#define LASER3_SPRITE 28
extern const uint8_t laser3_sprite[3] PROGMEM;
#define PLAYER_2 29
extern const uint8_t player_2[35] PROGMEM;
#define BULLET_SPRITE 30
extern const uint8_t bullet_sprite[7] PROGMEM;
#define VESSEL_0 31
extern const uint8_t vessel_0[18] PROGMEM;
#define HEALTHBAR_0 32
extern const uint8_t healthbar_0[86] PROGMEM;

uint8_t getSpriteIndex(uint8_t* sprite);
#endif