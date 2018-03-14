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

Environment newEnvironment();

void updateEnvironment(Environment env);

void removeObject(Environment env, Object instance);

void flushObjectList(Environment env);

Object getEnemySlot(Environment env);

Object getProjectileSlot(Environment env);

#endif /* MASTER_ENVIRONMENT_H_ */
