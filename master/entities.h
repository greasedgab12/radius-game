/*
 * entities.h
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_H_
#define MASTER_ENTITIES_H_
#include "structure.h"
#include "inttypes.h"

void playerScript(Object self, Environment mainEnv);

typedef struct PlayerEnv_Struct* PlayerEnv;

struct PlayerEnv_Struct{
	uint8_t health;
	uint8_t energy;
	uint8_t defense;
	//Velocity variables
	uint8_t acceleration;
	uint8_t v_max;
	int8_t v_x;
	int8_t v_y;
	uint16_t v_time;
	uint8_t v_delay;
	//Rate of fire variables
	uint16_t rof_time;
	uint8_t rof_delay;
};

PlayerEnv newPlayerEnv();

Object newPlayer(uint8_t x, uint8_t y);

void playerThink(Object self, Environment mainEnv);

uint8_t playerCollide(Object self, Object other);

void noOp();

uint8_t noCollide(Object self, Object other);

uint8_t simpleCollide(Object self, Object other);



#endif /* MASTER_ENTITIES_H_ */
