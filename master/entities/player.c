/*
 * player.c

 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#include <entities/general.h>
#include <entities/projectile.h>
#include <entity.h>
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"
#include "sprites.h"


#include "entities/player.h"
#include "entity.h"

Object newPlayer(uint8_t x, uint8_t y, const uint8_t* sprite){
	Object self = newObject(x,y,0,0,sprite);
	self->lx = self->slx;
	self->ly = self->sly*4;
	self->type = PLAYER;
	self->entity = newEntity();
	self->entity->health = 30;
	self->entity->maxHealth = 30;
	self->entity->maxEnergy=100;
    self->entity->energy = 0;
    self->entity->armor = 1;

    //Energy regeneration value
    self->entity->param1 = 2;


    self->entity->v_max = 50 + FRICTION;
    self->entity->v_x = 0;
    self->entity->v_y = 0;



	self->think = &playerThink;
	self->collide =&playerCollide;

	return self;


}

void playerThink(Object self, Environment mainEnv){
	//Movement
	int8_t a_x = 0, a_y = 0;
	//Button reactions
	/**D-Pad
	 * Only non opposing directions can be applied to the acceleration.
	**/

	if(mainEnv->buttons & M_U){
		a_y = -(self->entity->v_max/5+FRICTION);
	}
	else if(mainEnv->buttons & M_D){
		a_y = (self->entity->v_max/5+FRICTION);
	}

	if(mainEnv->buttons & M_L){
		a_x = -(self->entity->v_max/5+FRICTION);
	}
	else if(mainEnv->buttons & M_R){
		a_x = (self->entity->v_max/5+FRICTION);
	}
	/**
	 * A and B buttons.
	 */
	if(mainEnv->buttons & M_A){
		if(self->entity->weaponA){
			self->entity->weaponA->fire(self->entity->weaponA, self, mainEnv);
		}
	}
	else if(mainEnv->buttons & M_B){
		if(self->entity->weaponB){
			self->entity->weaponB->fire(self->entity->weaponB, self, mainEnv);
		}
	}

	/**
	 * Calculate movement of the player.
	 */

	/**Apply acceleration to velocity
	 * Velocity in each direction cannot exceed maximum velocity.
	 */
	//x-direction
	if(self->entity->energy <self->entity->maxEnergy){
		self->entity->energy+= self->entity->param1;
	}
	else{
		self->entity->energy = self->entity->maxEnergy;
	}
	if( abs(self->entity->v_x + a_x) < self->entity->v_max){
			self->entity->v_x += a_x;
		}
	//y-direction
	if( abs(self->entity->v_y + a_y) < self->entity->v_max){
		self->entity->v_y += a_y;
	}


	//Apply Friction to velocity
	//x-direction
	if(self->entity->v_x > FRICTION){
		self->entity->v_x -=FRICTION;
	}
	else if( self->entity->v_x < -FRICTION){
		self->entity->v_x +=FRICTION;
	}
	else{
		self->entity->v_x = 0;
	}
	//y-direction
	if(self->entity->v_y > FRICTION){
		self->entity->v_y -=FRICTION;
	}
	else if( self->entity->v_y < -FRICTION){
		self->entity->v_y +=FRICTION;
	}
	else{
		self->entity->v_y = 0;
	}
	//Apply velocitiy to position.
	moveObject(self, mainEnv,(self->entity->v_x)/10,(self->entity->v_y)/10);
	//Add the remainder to the next step:

}

uint8_t playerCollide(Object self, Object other,uint8_t cType, uint8_t iter){

	if(other){
		if(other->type == ENEMY || other->type == ENEMY_PROJECTILE){
			if(other->type == ENEMY){
				rebound(self,other, cType);
			}
			if(self->entity->health<other->entity->armor){
				self->entity->health = 0;
			}
			else{
				self->entity->health -= other->entity->armor;
			}
			if(self->entity->health ==0){
				self->killedBy =ENEMY;
			}

			if(iter){
				return 1;
			}
			else{

				return other->collide(other,self,cType,1);
			}
		}
		else{
			if(iter){
				return 0;
			}
			else{
				return other->collide(other,self,cType,1);
			}
		}
	}
	else{
		return 1;
	}

}
