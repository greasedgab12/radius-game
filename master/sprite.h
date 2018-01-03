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
    const uint8_t *data;
    //methods
    void (*setXY)(Sprite self, uint8_t x, uint8_t y);
    void (*setData)(Sprite self, const uint8_t *data);
};

struct Blank_Struct
{
    //attributes
    uint8_t x;
    uint8_t y;
    uint8_t lx;
    uint8_t ly;
};

/* newSprite: allocate memory for a sprite with initial data.
 * uint8_t x: x-coordinate of sprite
 * uint8_t y: y-coordinate of sprite
 * uint8_t *data: pointer to data array in memory
 * return Sprite: returns pointer to newly allocated sprite.
 */
Sprite newSprite(uint8_t x, uint8_t y, const uint8_t *data);

/* Setter methods for Sprite
 * setSpriteXY: Change position values of Sprite. This is does not equal moving the sprite.
 */
void setSpriteXY(Sprite self, uint8_t x, uint8_t y);
void setSpriteData(Sprite self, const uint8_t *data);

/* delSprite: Remove Sprite from spriteList and add deletion entry to delList
 * Sprite instance: Sprite to be deleted
 */
void delSprite(Sprite instance);

/* moveSprite: Move Sprite to new Position. Calls delSprite and moveSprite in succession.
 * Sprite instance: Sprite to be moved
 */
void moveSprite(Sprite instance, uint8_t nx, uint8_t ny);

/* drawSprite: Add Sprite to drawList.
 * Sprite instance: Sprite to be drawn.
 */
void drawSprite(Sprite instance);

/* mapSprite: Map the sprite data to the pages of the display.
 * Sprite instance: Sprite to be mapped to pages.
 * return Sprite: Resulting mapped Sprite
 */
Sprite mapSprite(Sprite instance);

/* newBlank: create Blank from Sprite.
 * Sprite instance: Sprite to be used as pattern.
 * return Blank: Generated instance of Blank.
 */
Blank newBlank(Sprite instance);


#endif // SPRITE_H_INCLUDED
