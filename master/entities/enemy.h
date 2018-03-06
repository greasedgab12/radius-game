/*
 * enemy.h
 *
 *  Created on: Mar 5, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_ENEMY_H_
#define MASTER_ENTITIES_ENEMY_H_

#include "structure.h"
#include "inttypes.h"


typedef struct EnemyEnv_Struct* EnemyEnv;

struct EnemyEnv_Struct{
	uint8_t health;
	uint8_t armor;
	//Velocity variables
	uint8_t acceleration;
	uint8_t v_max;
	int8_t a_x;
    int8_t a_y;
	int8_t v_x;
	int8_t v_y;
	int8_t s_x;
	int8_t s_y;

	uint8_t state;

	uint8_t param1;
	uint8_t param2;

	uint16_t v_time;
	uint8_t v_delay;
	//Rate of fire variables
	uint16_t rof_time;
	uint8_t rof_delay;
};

EnemyEnv newEnemyEnv();

Object newEnemyGlider(uint8_t y, uint8_t f);
Object newEnemyFloater(uint8_t x, uint8_t y);
Object newEnemyTracker(uint8_t x, uint8_t y);


void enemyThink(Object self, Environment mainEnv);

void enemyGliderThink(Object self, Environment mainEnv);

uint8_t enemyCollide(Object self, Object other, uint8_t cType, uint8_t iter);


#endif /* MASTER_ENTITIES_ENEMY_H_ */
