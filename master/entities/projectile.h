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
#include "sprite.h"




void newProjectile(Object self, uint8_t projectileType);



Object newBullet(uint8_t x, uint8_t y, int8_t v_x, int8_t v_y, uint8_t damage);

void bulletThink(Object self, Environment mainEnv);

uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter);


#endif /* MASTER_ENTITIES_PROJECTILE_H_ */
