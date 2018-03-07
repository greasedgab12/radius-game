/*
 * enemy.h
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_ENEMY_H_
#define MASTER_ENTITIES_ENEMY_H_
#include "structure.h"


Object newEnemy(uint8_t x, uint8_t y);

Object newEnemyGlider(uint8_t y, uint8_t f);
Object newEnemyFloater(uint8_t x, uint8_t y);
Object newEnemyTracker(uint8_t x, uint8_t y);


void enemyThink(Object self, Environment mainEnv);

void enemyGliderThink(Object self, Environment mainEnv);

uint8_t enemyCollide(Object self, Object other, uint8_t cType, uint8_t iter);

#endif /* MASTER_ENTITIES_ENEMY_H_ */
