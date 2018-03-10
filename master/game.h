/*
 * game.h
 *
 *  Created on: Mar 8, 2018
 *      Author: root
 */

#ifndef MASTER_GAME_H_
#define MASTER_GAME_H_

#include "structure.h"

GameState newGame();

GameState loadGame();

Object playerFromGameState(Environment mainEnv, GameState state);


#endif /* MASTER_GAME_H_ */
