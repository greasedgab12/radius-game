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
Object getPlayerFromGameState(Environment mainEnv);

void getNextEnemy(Environment mainEnv);

uint16_t getPoints(Environment mainEnv, Object enemy);

void drawHud(uint8_t health,uint8_t maxHealth, uint8_t energy,uint8_t maxEnergy, uint32_t points);

void setSpawnList(Environment env);

uint8_t isSpawnListEmpty(Environment env);

void getNextEnemy(Environment env);

#endif /* MASTER_GAME_H_ */
