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
	Environment self = (Environment)malloc(sizeof(struct Environment_struct));

	self->buttons =0;
	self->lastTime =0;
	self->time = 0;
    self->gameState =0;

    self->objectList = (Object*)malloc(sizeof(Object)*(MAXOBJECTS));
    uint8_t i;

    for(i=0; i<MAXOBJECTS; i++){
    	self->objectList[i]=(Object)malloc(sizeof(struct Object_Struct));
    	initObject(self->objectList[i]);
    	self->objectList[i]->entity =(Entity)malloc(sizeof(struct Entity_Struct));

    }
    self->weaponA = (Weapon)malloc(sizeof(struct Weapon_Struct));
    self->weaponB = (Weapon)malloc(sizeof(struct Weapon_Struct));
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


