/*
 * environment.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#include "structure.h"
#include "defines.h"
#include "environment.h"

#include "object.h"

#include "weapon.h"

#include "timer.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "game.h"


//Allocate Memory for all objects of the game.
Object objectList[MAXOBJECTS];
struct Object_Struct objects[MAXOBJECTS];
struct Entity_Struct  entityList[MAXOBJECTS];
struct Weapon_Struct weaponA;
struct Weapon_Struct weaponB;
struct Environment_struct environment;
struct GameState_Struct gameState;

uint8_t inputBuffer =0;

//ISR reads button inputs into inputBuffer.
SIGNAL (TIMER0_COMPA_vect){
	inputBuffer =0;
	if(B_UP){inputBuffer|=M_U;}
	if(B_DOWN){inputBuffer|=M_D;}
	if(B_LEFT){inputBuffer|=M_L;}
	if(B_RIGHT){inputBuffer|=M_R;}
	if(B_A){inputBuffer|=M_A;}
	if(B_B){inputBuffer|=M_B;}
	if(B_PAUSE){inputBuffer|=M_P;}
	if(B_SELECT){inputBuffer|=M_S;}
}



Environment newEnvironment(){
	Environment self = &environment;

	self->buttons =0;
	self->lastTime =0;
	self->time = 0;
    self->gameState =0;

    self->objectList = objectList;

    uint8_t i;
    //Set and initialize objects.
    for(i=0; i<MAXOBJECTS; i++){
    	self->objectList[i]=&objects[i];
    	initObject(self->objectList[i]);
    	self->objectList[i]->entity = &entityList[i];

    }
    self->weaponA = &weaponA;
    self->weaponB = &weaponB;
    self->gameState = &gameState;
    newGame(self->gameState);

    self->player=self->objectList[0];

    self->level =1;
    self->enemyRemaining =0;
    self->enemyCount = 0;
    self->enemyMax =0;

    for(i=0; i<4; i++){
    	self->spawnList[i]=0;
    	self->spawnDelay[i]=0;
    }
    self->points =0;

    return self;
}

void updateEnvironment(Environment env){
	//Load inputBuffer into env->buttons.
	cli();
	env->buttons = inputBuffer;
	sei();

	//Save old time to calculate time passed since last call.
	env->lastTime = env->time;
	//Cap time at approximately 30fps
	env->time = getMsTimer()/34;
}

void removeObject(Environment self, Object instance){
	uint8_t i;
	//Search and makr given object instance as empty:
	for(i=0; i<MAXOBJECTS; i++){
		if(self->objectList[i] == instance){
			self->objectList[i]->activeState =EMPTY;
		}
	}
}

void flushObjectList(Environment env){

	uint8_t i;
	for(i=0; i<MAXOBJECTS; i++){
		if(env->objectList[i]){
			env->objectList[i]->activeState= EMPTY;
			env->objectList[i]->drawState=NOTDRAWN;

		}
	}
	for(i=0; i<4; i++){
		env->spawnDelay[i]=0;
		env->spawnList[i]=0;
	}
}

Object getEnemySlot(Environment env){
	uint8_t i;
	for(i=1; i<=MAXENEMIES+1; i++){
		if(env->objectList[i]->activeState==EMPTY){
			return env->objectList[i];
		}
	}
	return 0;
}

Object getProjectileSlot(Environment env){
	uint8_t i;
	for(i=1+MAXENEMIES; i<1+MAXENEMIES+MAXPROJECTILES; i++){
		if(env->objectList[i]->activeState==EMPTY){
			return env->objectList[i];
		}
	}
	return 0;
}



