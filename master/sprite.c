#include "sprite.h"
#include "inttypes.h"
#include <stdlib.h>
#include "char.h"

//check if sprite already in ram
uint8_t* load_sprite(uint8_t *sprite)
{
	uint8_t sprite_nr;

	if(sprite == dino){
		sprite_nr = 0;
	}
	else if(sprite == enemy_0){
		sprite_nr =1;
	}
	else if(sprite==vessel_0){
		sprite_nr = 2;
	}
	else if(sprite==projectile_1){
		sprite_nr = 3;
	}
	else{
		sprite_nr = 4;
	}


	if(sprite_in_ram[sprite_nr] == 0)
	{
		uint8_t size = pgm_read_byte_near(&(sprite[1])) + 2;
		sprite_in_ram[sprite_nr] = (uint8_t*)malloc(size);
		memcpy_P(sprite_in_ram[sprite_nr], sprite, size);
	}
	return sprite_in_ram[sprite_nr];
}

void flush_sprite(uint8_t *sprite)
{
	uint8_t sprite_nr;
	if(sprite == dino)sprite_nr = 0;
	if(sprite == enemy_0)sprite_nr = 1;

	if(sprite_in_ram[sprite_nr] != 0)
	{
		free(sprite_in_ram[sprite_nr]);
		sprite_in_ram[sprite_nr] = 0;
	}
}

const uint8_t enemy_1[]  PROGMEM = {9,27, 	 //enemy_1.bmp
0x0,0xf,0x0,0xc0,0x30,0x0,0x0,0xf,0x0,
0xc0,0x3f,0x0,0x3c,0xcf,0x3,0x33,0xcf,0xc,
0xc3,0x3f,0xc,0x33,0xcf,0xc,0xc,0x0,0x3
};

const uint8_t vessel_0[]  PROGMEM = {8,16, 	 //vessel_0.bmp
0xc0,0x3,0xd0,0x3,0xf0,0x3,0xf0,0x1,
0xf4,0x5,0xf4,0x5,0xf5,0x15,0xf5,0x15
};

const uint8_t dino[]  PROGMEM = {28,196, 	 //dino.bmp
0xff,0xff,0xff,0xff,0xff,0xff,0x3f,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0xc,0xc,0xc,0x30,
0x3,0x0,0x0,0xc,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc0,0xc,0x30,0x3,0x0,0xf0,0xff,0xc0,0xc,0x30,
0x3,0x0,0x0,0xff,0xc3,0xc,0x30,0x3,0x0,0x0,0xfc,0xf,0x0,0x30,0x3,0x0,0x0,0xf0,0x3f,0x0,0x30,0x3,0x0,0x0,0xf0,0xff,0x0,0x30,
0x3,0x0,0x0,0xfc,0xff,0xff,0x30,0x3,0x0,0x0,0xff,0xff,0xcf,0x30,0x3,0x0,0xc0,0xff,0xff,0x3,0x30,0x3,0x0,0xf0,0xff,0xff,0x0,0x30,
0x3,0xff,0xff,0xff,0xff,0x3,0x30,0xc3,0xff,0xff,0xff,0xff,0xff,0x30,0xc3,0xfc,0xff,0xff,0x3f,0xc0,0x30,0xc3,0xff,0xff,0xff,0xf,0x0,0x30,
0xc3,0xff,0xff,0xff,0x0,0x0,0x30,0xc3,0xff,0xf,0x3,0x0,0x0,0x30,0xc3,0xff,0xc,0xf,0x0,0x0,0x30,0xc3,0xff,0xc,0x0,0x0,0x0,0x30,
0x3,0xff,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0xff,0xff,0xff,0xff,0xff,0xff,0x3f
};

const uint8_t player_4[]  PROGMEM = {9,27, 	 //player_4.bmp
0xcc,0x3f,0x3,0x30,0xcf,0x0,0xc0,0x30,0x0,
0xf0,0xf0,0x0,0xc,0xf,0x3,0xcc,0x30,0x3,
0x30,0xcf,0x0,0xc0,0x30,0x0,0x0,0xf,0x0
};

const uint8_t radius[]  PROGMEM = {40,200, 	 //radius.bmp
0xff,0xff,0xff,0xff,0xff,0x3,0x0,0x0,0x0,0xc0,0xc3,0xff,0xff,0xff,0xc3,0xc3,0x0,0x3,0x0,0xc0,0xc3,0x0,0x3,0x0,0xc0,0xc3,0x0,0xf,0x0,0xc0,0xc3,0x0,0xf3,0x0,0xc0,0x3,0xff,0x0,0xff,0xc3,
0x3,0x0,0x0,0x0,0xc0,0x3,0x0,0x0,0x0,0xc0,0x3,0x0,0x0,0x0,0xc0,0x3,0xf0,0xff,0xff,0xc3,0x3,0xf,0x0,0x3,0xc0,0xc3,0x0,0x0,0x3,0xc0,0xc3,0x0,0x0,0x3,0xc0,0x3,0xf,0x0,0x3,0xc0,
0x3,0xf0,0xff,0xff,0xc3,0x3,0x0,0x0,0x0,0xc0,0xc3,0xff,0xff,0xff,0xc3,0xc3,0x0,0x0,0x0,0xc3,0xc3,0x0,0x0,0x0,0xc3,0x3,0x3,0x0,0xc0,0xc0,0x3,0xc,0x0,0x30,0xc0,0x3,0xf0,0xff,0xf,0xc0,
0x3,0x0,0x0,0x0,0xc0,0x3,0xc0,0xfc,0xff,0xc3,0x3,0x0,0x0,0x0,0xc0,0xc3,0xff,0xff,0xff,0xc0,0x3,0x0,0x0,0x0,0xc3,0x3,0x0,0x0,0x0,0xc3,0x3,0x0,0x0,0x0,0xc3,0x3,0x0,0x0,0x0,0xc3,
0xc3,0xff,0xff,0xff,0xc0,0x3,0x0,0x0,0x0,0xc0,0x3,0x0,0x3f,0x30,0xc0,0x3,0xc0,0xc0,0xc0,0xc0,0x3,0xc0,0xc0,0xc0,0xc0,0x3,0x0,0x3,0x3f,0xc0,0x3,0x0,0x0,0x0,0xc0,0xff,0xff,0xff,0xff,0xff
};

const uint8_t player_2[]  PROGMEM = {9,27, 	 //player_2.bmp
0xf0,0xf0,0x0,0xc0,0x3f,0x0,0x0,0xf,0x0,
0xf,0xf,0xf,0x3f,0xcf,0xf,0xfc,0xff,0x3,
0xf0,0xff,0x0,0xc0,0x3f,0x0,0x0,0xf,0x0
};

const uint8_t enemy_2[]  PROGMEM = {9,27, 	 //enemy_2.bmp
0x0,0xf,0x0,0xc0,0x30,0x0,0xc0,0x3f,0x0,
0xf0,0xf0,0x0,0xcc,0x30,0x3,0xcc,0x30,0x3,
0x33,0xcf,0xc,0xc3,0x30,0xc,0x3f,0xc0,0xf
};

const uint8_t player_3[]  PROGMEM = {9,27, 	 //player_3.bmp
0x30,0xcf,0x0,0xcc,0x30,0x3,0xc3,0x30,0xc,
0xf3,0xff,0xc,0x3c,0xcf,0x3,0xc,0xf,0x3,
0x30,0xcf,0x0,0xc0,0x30,0x0,0x0,0xf,0x0
};

const uint8_t projectile_0[]  PROGMEM = {5,5, 	 //projectile_0.bmp
0x3c,0x3c,0x3c,0x3c,0x3c
};

const uint8_t enemy_0[]  PROGMEM = {9,27, 	 //enemy_0.bmp
0xf0,0xf0,0x0,0xc,0xf,0x3,0xfc,0xf0,0x3,
0xfc,0xff,0x3,0xcc,0x30,0x3,0xc,0xf,0x3,
0xcc,0x30,0x3,0x3c,0xcf,0x3,0x30,0xcf,0x0
};

const uint8_t shade[]  PROGMEM = {20,80, 	 //shade.bmp
0xff,0xff,0xff,0x3,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,
0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,
0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,
0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xaa,0x55,0x0,0xff,0xff,0xff,0x3
};

const uint8_t projectile_1[]  PROGMEM = {4,4, 	 //projectile_1.bmp
0x0f,0x0f,0x0f,0x0f
};

const uint8_t projectile_2[]  PROGMEM = {6,12, 	 //projectile_2.bmp
0xfc,0xf,0xfc,0xf,0xfc,0xf,
0xfc,0xf,0xfc,0xf,0xf0,0x3
};

const uint8_t enemy_4[]  PROGMEM = {9,27, 	 //enemy_4.bmp
0x0,0xf,0x0,0xc0,0x30,0x0,0x3c,0xcf,0x3,
0x33,0xcf,0xc,0x33,0xcf,0xc,0x33,0xcf,0xc,
0xcf,0x30,0xf,0x33,0xcf,0xc,0xf,0xf,0xf
};

const uint8_t enemy_3[]  PROGMEM = {9,27, 	 //enemy_3.bmp
0x0,0xf,0x0,0xc0,0x30,0x0,0x30,0xcf,0x0,
0x30,0xcf,0x0,0x3c,0xcf,0x3,0x33,0xcf,0xc,
0xc3,0x30,0xc,0x33,0xc0,0xc,0xf,0x0,0xf
};

const uint8_t player_1[]  PROGMEM = {9,27, 	 //player_1.bmp
0x3f,0xc0,0xf,0xfc,0xf0,0x3,0xfc,0xff,0x3,
0xf0,0xf0,0x0,0xf0,0xff,0x0,0xc0,0x3f,0x0,
0xc0,0x3f,0x0,0xc0,0x30,0x0,0x0,0xf,0x0
};

const uint8_t player_0[]  PROGMEM = {9,27, 	 //player_0.bmp
0x3c,0xc0,0x3,0xf0,0xf0,0x0,0xfc,0xff,0x3,
0xff,0xf0,0xf,0xf3,0xff,0xc,0xc3,0x3f,0xc,
0x0,0xf,0x0,0xc0,0x30,0x0,0x0,0xf,0x0
};

const uint8_t projectile_3[]  PROGMEM = {8,8, 	 //projectile_3.bmp
0xc3,0xc3,0xff,0x3c,0x3c,0x3c,0x3c,0x3c
};




