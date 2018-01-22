#ifndef MASTER_OBJECT_H_
#define MASTER_OBJECT_H_

#include "block.h"


typedef struct Environment_struct* Environment;
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
    void (*move)(Object self, Environment mainEnv, uint8_t x, uint8_t y);

    /* Whilst move will stay the same for any type of object, think and collide differ.
     * I.e. think for the player object will access the pressed buttons and a projectile
     * will only care for the elapsed time. Similarly, collide will trigger diffrent actions
     * depending on the involved objects.
    */
    void (*think)(Object self, Environment mainEnv);
    uint8_t (*collide)(Object self, Object other);
};


struct Environment_struct{
	uint8_t buttons;
	uint32_t time;
	uint8_t gameState;
	Object* objectList;
	uint8_t oPos;
	Block* blockList;
	uint8_t bPos;
};

/* newObject: allocate memory for a object with initial data.
 * uint8_t x: x-coordinate of Object.
 * uint8_t y: y-coordinate of Object.
 * uint8_t *data: pointer to data array in memory.
 * return Object: returns pointer to newly allocated Object.
 */
Object newObject(uint8_t x, uint8_t y, const uint8_t *data);

/* free memory of Object.
 */
void releaseObject(Object instance);

/* Setter methods for object
 * setObjectXY: Change position values of object. This is does not equal moving the object.
 */
void setObjectXY(Object self, uint8_t x, uint8_t y);
void setObjectData(Object self, const uint8_t *data);

/* moveObject: Move the Object to the given location. Accesses blockList of main environment.
 * Object self: Object to be moved.
 * Environment mainEnv: main environment. blockList is manipulated to force redraw of previously occupied pages.
 * uint8_t x: x-coordinate to move to.
 * uint8_t y: y-coordinate to move to.
 */
void moveObject(Object self, Environment mainEnv, uint8_t x, uint8_t y);


/* mapObject: Create new Block from Object instance
 * Object instance: Object to be mapped to a Block of pages.
 * return Block: Resulting Block.
 */
Block mapObject(Object instance);



#endif /* MASTER_OBJECT_H_ */
