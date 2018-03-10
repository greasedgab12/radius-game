/*
 * player.h
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_PLAYER_H_
#define MASTER_ENTITIES_PLAYER_H_
#include "structure.h"
#include "inttypes.h"

Object newPlayer(uint8_t x, uint8_t y, const uint8_t* sprite);

void playerThink(Object self, Environment mainEnv);

uint8_t playerCollide(Object self, Object other,uint8_t cType, uint8_t iter);


#endif /* MASTER_ENTITIES_PLAYER_H_ */
