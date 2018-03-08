#include "sprite.h"
#include "inttypes.h"
#include <stdlib.h>
#include "char.h"

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
	uint8_t sprite_nr = getSpriteIndex(sprite);

	if(sprite_in_ram[sprite_nr] != 0)
	{
		free(sprite_in_ram[sprite_nr]);
		sprite_in_ram[sprite_nr] = 0;
	}
}





