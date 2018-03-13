#include "sprite.h"
#include "inttypes.h"
#include <stdlib.h>
#include "char.h"
#include "sprites.h"
#include "display.h"

uint8_t* sprite_in_ram[MAX_SPRITE_COUNT];

void spriteInit(){
	uint8_t i;
	for(i=0; i<MAX_SPRITE_COUNT; i++){
		sprite_in_ram[i]=0;
	}
}

//check if sprite already in ram
uint8_t* load_sprite(const uint8_t *sprite)
{

	uint8_t sprite_nr = getSpriteIndex(sprite);

	if(sprite_in_ram[sprite_nr] == 0)
	{
		uint8_t size = pgm_read_byte_near(&(sprite[1])) + 2;
		sprite_in_ram[sprite_nr] = (uint8_t*)malloc(size);
		memcpy_P(sprite_in_ram[sprite_nr], sprite, size);
	}
	return sprite_in_ram[sprite_nr];
}

void flush_sprite(const uint8_t *sprite)
{
	if(sprite == 0){
		return;
	}
	uint8_t sprite_nr = getSpriteIndex(sprite);

	if(sprite_in_ram[sprite_nr] != 0)
	{
		free(sprite_in_ram[sprite_nr]);
		sprite_in_ram[sprite_nr] = 0;
	}
}


void drawTitleScreen(){
	uint8_t *title = (uint8_t*)malloc(sizeof(uint8_t)*96*9+2);
	memcpy_P(title, radius2, 96*9+2);
	sendWindow(34,5,96,9,title);
	free(title);

}

void flushAllSprites(){
	uint8_t i;
	for(i=0; i<MAX_SPRITE_COUNT; i++){
		flush_sprite(sprite_in_ram[i]);
	}
}


