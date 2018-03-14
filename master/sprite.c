#include "sprite.h"
#include "inttypes.h"
#include <stdlib.h>
#include "char.h"
#include "sprites.h"
#include "display.h"

#include <util/delay.h>

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

void flush_sprite( const uint8_t *sprite)
{
	if(sprite == 0){
		return;
	}
	uint8_t i;
	for(i=0; i<MAX_SPRITE_COUNT; i++){
		if(sprite_in_ram[i] == sprite){
			free(sprite);
			sprite_in_ram[i] = 0;
		}
	}

}


void drawTitleScreen(){
	uint8_t i;
	for(i=0; i<95; i++){
		uint8_t *title = (uint8_t*)malloc(sizeof(uint8_t)*9);
		memcpy_P(title, radius +9*i, 18);
		_delay_ms(20);
		sendWindow(34+i,5,1,9,title);
		free(title);
	}


}

void flushAllSprites(){
	uint8_t i;
	for(i=0; i<MAX_SPRITE_COUNT; i++){
		flush_sprite(sprite_in_ram[i]);
	}
}


