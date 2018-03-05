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
#include "buttons.h"
#include "structure.h"
#include "environment.h"
#include "defines.h"
#include "object.h"

Environment newEnvironment(Object* objectList){
	Environment self = (Environment)malloc(sizeof(struct Environment_struct));
    self->time = getMsTimer();
    self->gameState =0;
    self->bPos = 0;
    self->objectList = objectList;
    self->oPos = 0;
    self->buttons =0;
    return self;
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
//ToDO: Fix the removal routine!
void removeObject(Environment self, Object instance){
	uint8_t i;

	//Search and remove given object isntance from list:
	for(i=0; i<self->oPos; i++){
		if(self->objectList[i] && self->objectList[i] == instance){
			self->objectList[self->oPos] =0;
			releaseObject(instance);
		}
	}

	for(i=0; i<self->oPos; i++){
		if(self->objectList[i]==0){
			uint8_t j;
			for(j=i; j<self->oPos-1; j++){
				printN(j,64,j*2);
				self->objectList[j] = self->objectList[j+1];
			}
			self->objectList[self->oPos-1]=0;
			self->oPos= self->oPos>0?self->oPos -1: 0;
			i= i>0?i-1:0;
		}
		printN(i,48,i*2);
	}

}

