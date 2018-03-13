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

    self->objectList = (Object*)malloc(sizeof(Object)*MAXOBJECTS);
    uint8_t i;
    for(i=0; i<MAXOBJECTS; i++){
    	self->objectList[i]=0;
    }

    self->oPos = 0;

    self->player=0;
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

void addObject(Environment self, Object instance){
	if(self->oPos<MAXOBJECTS){
		self->objectList[self->oPos] = instance;
		self->oPos++;
	}
	else{
		releaseObject(instance);
	}
}
void removeObject(Environment self, Object instance){
	uint8_t i;

	//Search and remove given object isntance from list:
	for(i=0; i<self->oPos; i++){
		if(self->objectList[i] == instance){
			releaseObject(instance);
			self->objectList[i] =0;
		}
	}

	for(i=0; i<self->oPos; i++){
		if(self->objectList[i]==0){
			uint8_t j;
			for(j=i; j<self->oPos-1; j++){
				self->objectList[j] = self->objectList[j+1];
			}
			self->objectList[self->oPos-1]=0;
			self->oPos= self->oPos>0?self->oPos -1: 0;
			i= i>0?i-1:0;
		}
	}

}

void flushObjectList(Environment env){

	uint8_t i;
	for(i=0; i<MAXOBJECTS; i++){
		if(env->objectList[i]){
			releaseObject(env->objectList[i]);
			env->objectList[i]=0;
		}
	}
	env->oPos =0;
}



