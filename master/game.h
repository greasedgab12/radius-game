/*
 * game.h
 *
 *  Created on: Mar 8, 2018
 *      Author: root
 */

#ifndef MASTER_GAME_H_
#define MASTER_GAME_H_

#include "structure.h"

/** newGame: Initialize gameState with initial values.
 *  GameState self: GameState instance.
 */
void newGame(GameState self);


/** getPlayerFromGameState: Reconstruct player character from GameState.
 *	Environment env: Environment instance.
 */
void getPlayerFromGameState(Environment env);

/**getNextEnemy: Activate next enemy the spawn list.
 * Environment env: Environment instance.
 */
void getNextEnemy(Environment env);

/**getPoints: Calculate points of enemy based on its attributes and current level.
 * Environment env: Environment instance.
 * Object enemy: Enemy instance.
 * return uint16_t: Points
 */
uint16_t getPoints(Environment env, Object enemy);

/**drawHud: Draw Hud (Health bar, Energy bar and points) into the upper area of the screen.
 * uint8_t health: Health of the player.
 * uint8_t maxHealth: Maximum health of the player.
 * uint8_t energy: Energy of the player.
 * uint8_t maxEnergy: Maximum energy of the player.
 * uint32_t points: Points gained in the current level.
 */
void drawHud(uint8_t health,uint8_t maxHealth, uint8_t energy,uint8_t maxEnergy, uint32_t points);

/**setSpawnList: Fill the spawn list with new enemies.
 * Environment env: Environment instance.
 */
void setSpawnList(Environment env);

/**isSpawnListEmpty: Checks wether the spawn list is empty.
 * Environment env: Environment instance.
 * return uint8_t: 0: not empty, 1: empty.
 */
uint8_t isSpawnListEmpty(Environment env);

/**getNextEnemy: Search spawn list for the next enemy to activate.
 * Environment env: Environment instance.
 */
void getNextEnemy(Environment env);

#endif /* MASTER_GAME_H_ */
