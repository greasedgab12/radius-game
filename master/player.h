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

/**newPlayer:Create blank player type object.
 * Object self: Memory slot for the object.
 * uint8_t* sprite: pointer to sprite in program memory
 * uint8_t x,y: coordinates of the new enemy type object.
 */
void newPlayer(Object self, uint8_t x, uint8_t y, const uint8_t* sprite);


/**playerThink: Player logic routine.
 * Object self: Player object.
 * Environment env: Pointer to game environment.
 */
void playerThink(Object self, Environment env);

/**playerCollide: Player collision routine.
 * Object self: Colliding player.
 * Object other: Other colliding object.
 * uint8_t cType: Collision type. See defines.h.
 * uint8_t iter: Collision iteration.
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t playerCollide(Object self, Object other,uint8_t cType, uint8_t iter);


#endif /* MASTER_ENTITIES_PLAYER_H_ */
