#ifndef SPRITE_H
#define SPRITE_H
#include <avr/pgmspace.h>
#include <inttypes.h>

#define MAX_SPRITE_COUNT 35

//array of pointers to sprite data
extern uint8_t* sprite_in_ram[MAX_SPRITE_COUNT];


/**load_sprite: load sprite out of program memory and return pointer to SRAM.
 * const uin8_t* sprite: Pointer to sprite in program memory.
 * return uint8_t*: Pointer to sprite in SRAM.
 */
uint8_t* load_sprite(const uint8_t* sprite);

/**flush_sprite: Deallocate loaded sprite in SRAM.
 * uint8_t* sprite: Pointer to sprite in SRAM.
 */
void flush_sprite( uint8_t* sprite);

/**flushAllSprites(): Deallocate all loaded Sprites
 *
 */
void flushAllSprites();

/**drawTitleScreen: Load an draw the Radius Logo, then deallocate.
 * This function draws the Radius logo since it's too big for the load_sprite function.
 */
void drawTitleScreen();

//Initialize sprite_in_ram array.
void spriteInit();

#endif
