/*
 * environment.h
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#ifndef MASTER_ENVIRONMENT_H_
#define MASTER_ENVIRONMENT_H_

//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
extern uint8_t inputBuffer;

//newEnvironment: Initialize environment.
Environment newEnvironment();

/**updateEnvironment: Update time and button attributes of the environment.
 * Environment env: Environment instance.
 */
void updateEnvironment(Environment env);

/**removeObject: Mark given object in environment object list as empty.
 * Environment env: Environment instance.
 * Object instance: Object instance.
 */
void removeObject(Environment env, Object instance);

/**flushObjectList: Mark all Objects of the environment as empty. Flush spawn lists also.
 * Enviroment env: Environment instance.
 */
void flushObjectList(Environment env);

/**getEnemySlot: Search and return a free slot for an enemy. Returns 0 if none is found.
 * Environment env: Environment instance.
 * return Object: Free slot.
 */
Object getEnemySlot(Environment env);

/**getProjectileSlot: Search and return a free slot for a projectile.  Returns 0 if none is found.
 * Environment env: Environment instance.
 * return Object: Free slot.
 */
Object getProjectileSlot(Environment env);

#endif /* MASTER_ENVIRONMENT_H_ */
