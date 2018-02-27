/*
 * structures.h
 *
 *  Created on: Feb 26, 2018
 *      Author: root
 */

#ifndef MASTER_STRUCTURE_H_
#define MASTER_STRUCTURE_H_
#include "inttypes.h"
#include "defines.h"

typedef struct Environment_struct* Environment;
typedef struct Object_Struct* Object;
typedef struct Block_struct* Block;

struct Block_struct{
	uint8_t x,y,lx,ly;
	uint8_t blockType;
	uint8_t *data;
};

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


#endif /* MASTER_STRUCTURE_H_ */
