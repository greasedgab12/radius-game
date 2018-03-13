/*
 * game.h
 *
 *  Created on: Mar 8, 2018
 *      Author: root
 */

#ifndef MASTER_GAME_H_
#define MASTER_GAME_H_

#include "structure.h"

/** newGame: Create gameState with initial values.
 *  return GameState
 */
GameState newGame();

/** loadGame: Load GameState structure from EEPROM.
 *  return GameState:
 */
GameState loadGame();

/** getPlayerFromGameState
 *
 */
Object getPlayerFromGameState(Environment mainEnv, GameState state);

void getNextEnemy(Environment mainEnv);

uint16_t getPoints(Environment mainEnv, Object enemy);

void drawHud(Environment mainEnv);

void setSpawnList(Environment env);

uint8_t isSpawnListEmpty(Environment env);

void getNextEnemy(Environment env);

#endif /* MASTER_GAME_H_ */
