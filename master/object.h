#ifndef MASTER_OBJECT_H_
#define MASTER_OBJECT_H_

#include "block.h"

struct Environment;

typedef struct Object_Struct* Object;

struct Object_Struct{
	//attributes
	uint8_t x,y,lx,ly;
	const uint8_t *data;
	Block representation;
	//Pointer to Object data aside from Position and Graphics i.e. HP of player.
	void* objectEnv;
	//Type of the Object. See Objectlist for types
	uint8_t type;

	//methods
    void (*setXY)(Object self, uint8_t x, uint8_t y);
    void (*setData)(Object self, const uint8_t *data);
    void (*think)(Object self, struct Environment *mainEnv);
    void (*move)(Object self, uint8_t x, uint8_t y);
    void (*collide)(Object self, Object other);
};
/* newObject: allocate memory for a sprite with initial data.
 * uint8_t x: x-coordinate of sprite
 * uint8_t y: y-coordinate of sprite
 * uint8_t *data: pointer to data array in memory
 * return Object: returns pointer to newly allocated Object.
 */
Object newObject(uint8_t x, uint8_t y, uint8_t type);

/* Setter methods for Sprite
 * setSpriteXY: Change position values of Sprite. This is does not equal moving the sprite.
 */
void setSpriteXY(Object self, uint8_t x, uint8_t y);
void setSpriteData(Object self, const uint8_t *data);

//Create new Block from Object instance
Block mapObject(Object instance);



#endif /* MASTER_OBJECT_H_ */
