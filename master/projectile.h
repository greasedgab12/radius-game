/*
 * bullet.h
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_PROJECTILE_H_
#define MASTER_ENTITIES_PROJECTILE_H_
#include "general.h"
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"
#include "sprite.h"


/**newProjectile: Create projectile object from projectile type.
 * Object self: Memory slot in game environment.
 * uint8_t projectileType: Type of the projectile. See defines.h.
 */
void newProjectile(Object self, uint8_t projectileType);



/**bulletThink: Logic routine of bullet type objects.
 * Object: projectile object
 * Environment env: Game environment.
 */
void bulletThink(Object self, Environment env);

/**discThink: Logic routine of disc type objects. Vertical friction is applied.
 * Object: projectile object
 * Environment env: Game environment.
 */
void discThink(Object self, Environment env);

/**missileThink: Logic routine of missile type objects.
 * Oldest enemy is set as target, then followed in y-Axis.
 * Object: projectile object
 * Environment env: Game environment.
 */
void missileThink(Object self, Environment env);

/**shotThink: Logic routine of shot and laser type objects. No movement.
 * Object: projectile object
 * Environment env: Game environment.
 */
void shotThink(Object self, Environment env);

/**bulletCollide: Standard collision logic of bullet type projectiles.
 * Object self: Colliding projectile.
 * Object other: Other colliding object.
 * uint8_t cType: Collision type. See defines.h.
 * uint8_t iter: Collision iteration.
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter);

/**ballCollide: Collision logic of ball type projectiles. Rebounds off other objects.
 * Object self: Colliding projectile.
 * Object other: Other colliding object.
 * uint8_t cType: Collision type. See defines.h.
 * uint8_t iter: Collision iteration.
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t ballCollide(Object self, Object other,uint8_t cType,uint8_t iter);

/**ballCollide: Collision logic of laser type projectiles. Collision reduces projectile health.
 * Object self: Colliding projectile.
 * Object other: Other colliding object.
 * uint8_t cType: Collision type. See defines.h.
 * uint8_t iter: Collision iteration.
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t laserCollide(Object self, Object other,uint8_t cType,uint8_t iter);

#endif /* MASTER_ENTITIES_PROJECTILE_H_ */
