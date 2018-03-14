/*
 * environment.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#include <stdlib.h>
#include "timer.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "structure.h"
#include "environment.h"
#include "defines.h"
#include "object.h"
#include "entities/general.h"

Object objectList[MAXOBJECTS];
struct Object_Struct objects[MAXOBJECTS];
struct Entity_Struct  entityList[MAXOBJECTS];
struct Weapon_Struct weaponA;
struct Weapon_Struct weaponB;
struct Environment_struct environment;
struct GameState_Struct gameState;

uint8_t inputBuffer =0;

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
	cli();
	env->buttons = inputBuffer;
	sei();

	env->lastTime = env->time;
	//Cap time at approximately 60fps
	env->time = getMsTimer()/34;
}

void removeObject(Environment self, Object instance){
	uint8_t i;
	//Search and remove given object isntance from list:
	for(i=0; i<MAXOBJECTS; i++){
		if(self->objectList[i] == instance){
			self->objectList[i]->activeState =0;
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

void newEffect(Environment env, uint8_t x, uint8_t y, uint8_t* effectType){
	uint8_t i;
	for(i=1+MAXENEMIES+MAXPROJECTILES; i<=MAXOBJECTS; i++){
		if(env->objectList[i]->activeState==EMPTY){
			env->objectList[i]->activeState=ACTIVE;
			newObject(env->objectList[i],x,y,0,0,effectType);
			env->objectList[i]->lx = env->objectList[i]->slx;
			env->objectList[i]->ly = env->objectList[i]->sly;
			env->objectList[i]->type = 0;
			env->objectList[i]->think = &noOp;
			env->objectList[i]->collide = &noCollide;
		}
	}
}


