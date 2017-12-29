#ifndef SPRITE_H
#define SPRITE_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#define MAXSPRITES 32
#define MAXDELETES 32
#define MAXOBJECTS 32

typedef struct Sprite_Struct* Sprite;
typedef struct Blank_Struct* Blank;





/*The byte-patterns for the sprite struct have the following shape: {lx, lx*ly, data...} */
//const uint8_t player[] PROGMEM = {4,8, 0xFF, 0xFF, 0x0C,0x30,0x30,0x0C,0xC0,0x03};



struct Sprite_Struct
{
    //attributes
    uint8_t x;
    uint8_t y;
    uint8_t lx;
    uint8_t ly;
    uint8_t *data;
    //methods
    void (*setXY)(Sprite self, uint8_t x, uint8_t y);
    void (*setData)(Sprite self, uint8_t *data);
};

struct Blank_Struct
{
    //attributes
    uint8_t x;
    uint8_t y;
    uint8_t lx;
    uint8_t ly;
};

void spriteInit();

Sprite newSprite(uint8_t x, uint8_t y, uint8_t *data);

void setSpriteXY(Sprite self, uint8_t x, uint8_t y);
void setSpriteData(Sprite self, uint8_t *data);

void delSprite(Sprite instance);
void moveSprite(Sprite instance, uint8_t nx, uint8_t ny);
void drawSprite(Sprite instance);

Blank newBlank(Sprite instance);

#endif // SPRITE_H_INCLUDED
