/*
 * player.c

 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#include "structure.h"
#include "defines.h"
#include "environment.h"

#include "entity.h"
#include "player.h"
#include "general.h"



void newPlayer(Object self, uint8_t x, uint8_t y, const uint8_t* sprite){
	//Set sprite to player object.
	self->setData(self,sprite);
	//Set physical collision to the sprite size.
	self->lx = self->slx;
	self->ly = self->sly*4;
	self->type = PLAYER;
	//Initialize entity variable.
	newEntity(self->entity);
	//Dummy values.
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

    //Set the function pointers of the structure.
	self->think = &playerThink;
	self->collide =&playerCollide;
	self->activeState = ACTIVE;


}

void playerThink(Object self, Environment env){
	//Movement
	int8_t a_x = 0, a_y = 0;
	//Button reactions
	/**D-Pad
	 * Only non opposing directions can be applied to the acceleration.
	**/

	if(env->buttons & M_U){
		a_y = -(self->entity->v_max/5+FRICTION);
	}
	else if(env->buttons & M_D){
		a_y = (self->entity->v_max/5+FRICTION);
	}

	if(env->buttons & M_L){
		a_x = -(self->entity->v_max/5+FRICTION);
	}
	else if(env->buttons & M_R){
		a_x = (self->entity->v_max/5+FRICTION);
	}
	/**
	 * A and B buttons.
	 */

	if(env->buttons & M_A){
		if(env->weaponA){
			env->weaponA->fire(env->weaponA, self, env);
		}
	}
	else if(env->buttons & M_B){
		if(env->weaponB){
			env->weaponB->fire(env->weaponB, self, env);
		}
	}
	//Energy regeneration.
	if(self->entity->energy <self->entity->maxEnergy){
		self->entity->energy+= self->entity->param1;
	}
	else{
		self->entity->energy = self->entity->maxEnergy;
	}


	/**
	 * Calculate movement of the player.
	 */

	/**Apply acceleration to velocity
	 * Velocity in each direction cannot exceed maximum velocity.
	 */
	//x-direction

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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Add the remainder to the next step:
	self->entity->s_x += self->entity->v_x%10;
	self->entity->s_y += self->entity->v_y%10;
	self->entity->s_x = self->entity->s_x%10;
	self->entity->s_y = self->entity->s_y%10;




}

uint8_t playerCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	if(other){
		if(other->type == ENEMY || other->type == ENEMY_PROJECTILE){
			//Is the other object of type enemy or enemy projectile, cause damage to the player.
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
			//Other objects have to know for themselves, what to do in the collision case.
			if(iter){
				return 0;
			}
			else{
				return other->collide(other,self,cType,1);
			}
		}
	}
	else{
		//Collision with border.
		return 1;
	}

}
