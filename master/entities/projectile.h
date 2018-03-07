/*
 * bullet.h
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_PROJECTILE_H_
#define MASTER_ENTITIES_PROJECTILE_H_
#include <entities/general.h>
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"

typedef struct BulletEnv_Struct* BulletEnv;

struct BulletEnv_Struct{
	uint8_t damage;
	//Velocity variables
	int8_t v_x;
	int8_t v_y;
	int8_t s_x;
	int8_t s_y;

	uint16_t v_time;
	uint8_t v_delay;
};

BulletEnv newBulletEnv(int8_t v_x, int8_t v_y, uint8_t damage);


Object newBullet(uint8_t x, uint8_t y, int8_t v_x, int8_t v_y, uint8_t damage);

void bulletThink(Object self, Environment mainEnv);

uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter);


#endif /* MASTER_ENTITIES_PROJECTILE_H_ */
