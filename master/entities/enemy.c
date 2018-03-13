/*
 * enemy.c
 *
 *  Created on: Mar 5, 2018
 *      Author: root
 */

/*
 * player.c

 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */

#include <entities/general.h>
#include <entities/projectile.h>
#include <entity.h>
#include <entity.h>
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"
#include "sprites.h"

#include "entity.h"
#include "entities/enemy.h"



Object newEnemy(uint8_t x, uint8_t y){
	Object self = newObject(x,y,9,9,enemy_0);
	self->type = ENEMY;
	self->entity = newEntity();
	self->think = &enemyThink;
	self->collide =&enemyCollide;

	return self;


}

Object newEnemyGlider(uint8_t y, uint8_t f){
	Object self = newObject(0,y,9,9,enemy_0);
	self->type = ENEMY;
	self->entity = newEntity();
	self->collide =&enemyCollide;
	self->think = &enemyGliderThink;

	self->setXY(self,MAXX -self->lx, y);
	self->entity->param1 = f;
	return self;


}
/**
Object newEnemyTracker(uint8_t y){
	Object self = newObject(0,y,9,9,enemy_0);
	self->type = ENEMY;
	self->entity = newEntity();
	self->think = &enemyThink;
	self->collide =&enemyCollide;

	return self;

}
**/
void enemyGliderThink(Object self, Environment mainEnv){
	//Movement
	if(self->killedBy!=0){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->entity->v_time + self->entity->v_delay){

		//Update time variable to correctly acquire time passed.
		self->entity->v_time = mainEnv->time;

		/**Apply acceleration to velocity
		 * Velocity in each direction cannot exceed maximum velocity.
		 */
		//x-direction
		if(self->entity->state<self->entity->param1){
			self->entity->state++;
		}
		else{
			self->entity->state =0;
		}
		/**
		self->entity->a_y += self->entity->a_y<10? (-1)*(self->entity->state/(self->entity->param1/2)*self->entity->state*10/self->entity->param1):0;
		**/
		if(self->entity->state < self->entity->param1/2){

			self->entity->a_y =1 + FRICTION;
		}
		else{
			self->entity->a_y =-1 -FRICTION;
		}




		if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
			self->entity->v_x += self->entity->a_x;
		}
		//y-direction
		if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
			self->entity->v_y += self->entity->a_y;
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
		moveObject(self, mainEnv,(self->entity->s_x+self->entity->v_x)/10,(self->entity->s_y+self->entity->v_y)/10);
		//Add the remainder to the next step:
		self->entity->s_x = (self->entity->s_x+self->entity->v_x)%10;
		self->entity->s_y = (self->entity->s_y+self->entity->v_y)%10;

	}

}


void enemyThink(Object self, Environment mainEnv){
	//Movement
	if(self->killedBy!=0){
		return;
	}

	if(((uint16_t) mainEnv->time) >= self->entity->v_time + self->entity->v_delay){

		//Update time variable to correctly acquire time passed.
		self->entity->v_time = mainEnv->time;
		/**Apply acceleration to velocity
		 * Velocity in each direction cannot exceed maximum velocity.
		 */
		//x-direction
		if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
			self->entity->v_x += self->entity->a_x;
		}
		//y-direction
		if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
			self->entity->v_y += self->entity->a_y;
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
		moveObject(self, mainEnv,(self->entity->s_x+self->entity->v_x)/10,(self->entity->s_y+self->entity->v_y)/10);
		//Add the remainder to the next step:
		self->entity->s_x = (self->entity->s_x+self->entity->v_x)%10;
		self->entity->s_y = (self->entity->s_y+self->entity->v_y)%10;

	}
}

uint8_t enemyCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	if(other){
		if(other->type ==PLAYER){

			if(self->entity->health<other->entity->armor){
				self->entity->health = 0;
			}
			else{
				self->entity->health -= other->entity->armor;
			}
			if(self->entity->health ==0){
				self->killedBy = PLAYER;
			}
			if(iter){
				return 1;
			}
			else{
				return other->collide(other, self, cType, 0);
			}
		}
		else if( other->type == PLAYER_PROJECTILE){
			rebound(self,other,cType);


			if(self->entity->health<other->entity->armor){
				self->entity->health = 0;
			}
			else{
				self->entity->health -= other->entity->armor;
			}
			if(self->entity->health ==0){
				self->killedBy =PLAYER;
			}
			if(iter){
				return 1;
			}
			else{
				return other->collide(other, self, cType, 1);
			}
		}
		else{
			if(iter){
				return 1;
			}
			else{
				return other->collide(other,self,cType,1);
			}
		}
	}
	else{
		if(cType == LEFTC){
			self->killedBy =BORDER;
		}
		else{
			rebound(self,0, cType);
			drag(self, 1);
		}
		return 1;
	}

}

