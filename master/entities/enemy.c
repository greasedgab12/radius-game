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
#include "weapon.h"

#include "entity.h"
#include "entities/enemy.h"



Object newEnemy(uint8_t * sprite,uint8_t x, uint8_t y){
	Object self = newObject(x,y,9,9,sprite);
	self->lx = self->slx;
	self->ly = self->sly*4;
	self->type = ENEMY;
	self->entity = newEntity();
	self->think = &enemyThink;
	self->collide =&enemyCollide;

	return self;


}

Object newEnemyFloater(uint8_t * sprite,uint8_t y){
	Object self = newEnemy(sprite,0,y);
	self->setXY(self,MAXX -self->lx, y);
	return self;
}

Object newEnemyGlider(uint8_t * sprite,uint8_t y, uint8_t f){
	Object self = newEnemy(sprite,0,y);
	self->think = &enemyGliderThink;
	self->setXY(self,MAXX -self->lx, y);
	self->entity->param1 = f;
	return self;


}

Object newEnemyTracker(uint8_t * sprite,uint8_t y){
	Object self = newEnemy(sprite,0,y);
	self->think = &enemyTrackerThink;
	self->setXY(self,MAXX -self->lx, y);
	return self;

}

Object newEnemyShooter(uint8_t * sprite,uint8_t y, uint8_t invProbability){
	Object self = newEnemy(sprite,0,y);
	self->think = &enemyShooterThink;
	self->entity->weaponA = newGun(0);
	self->entity->param1 = invProbability;
	self->setXY(self,MAXX -self->lx, y);
	return self;

}

void enemyThink(Object self, Environment mainEnv){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}


	//Apply accelertation to velocity.
	//x-direction
	if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
		self->entity->v_x += self->entity->a_x;
	}
	else if ( abs(self->entity->v_x)<= self->entity->v_max){
		if(self->entity->v_x>0){
			self->entity->v_x = self->entity->v_max;
		}
		else{
			self->entity->v_x = -self->entity->v_max;
		}
	}
	//y-direction
	if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
		self->entity->v_y += self->entity->a_y;
	}
	else if ( abs(self->entity->v_y)<= self->entity->v_max){
		if(self->entity->v_y>0){
			self->entity->v_y = self->entity->v_max;
		}
		else{
			self->entity->v_y = -self->entity->v_max;
		}
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
	//Apply velocity to position.
	moveObject(self, mainEnv,(self->entity->v_x)/10,(self->entity->v_y)/10);



}


void enemyGliderThink(Object self, Environment mainEnv){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}

	//Calculate acceleration based on the state (param1) of the entity.
	if(self->entity->state<self->entity->param1){
		self->entity->state++;
	}
	else{
		self->entity->state =0;
	}
	//Accelerate either upwards or downwards.
	if(self->entity->state < self->entity->param1/2){

		self->entity->a_y =1 + FRICTION;
	}
	else{
		self->entity->a_y =-1 -FRICTION;
	}


	//Apply accelertation to velocity.
	//x-direction
	if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
		self->entity->v_x += self->entity->a_x;
	}
	else if ( abs(self->entity->v_x)<= self->entity->v_max){
		if(self->entity->v_x>0){
			self->entity->v_x = self->entity->v_max;
		}
		else{
			self->entity->v_x = -self->entity->v_max;
		}
	}
	//y-direction
	if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
		self->entity->v_y += self->entity->a_y;
	}
	else if ( abs(self->entity->v_y)<= self->entity->v_max){
		if(self->entity->v_y>0){
			self->entity->v_y = self->entity->v_max;
		}
		else{
			self->entity->v_y = -self->entity->v_max;
		}
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
		//Apply velocity to position.
	moveObject(self, mainEnv,(self->entity->v_x)/10,(self->entity->v_y)/10);
}

void enemyTrackerThink(Object self, Environment mainEnv){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}
	Object target=mainEnv->player;
	//Search for a valid target.


	//Calculate acceleration based on target position.
	//The tracker enemy can only accelerate in the y-axis.
	if(target){
		if(self->y < target->y){
			self->entity->a_y = 10 + FRICTION;
		}
		else if(self->y + self->ly > target->y){
			self->entity->a_y = -(10 + FRICTION);
		}
		else{
			self->entity->a_y =0;
		}
	}

	//Apply acceleration to velocity.
	//y-direction
	if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
		self->entity->v_x += self->entity->a_x;
	}
	else if ( abs(self->entity->v_x)<= self->entity->v_max){
		if(self->entity->v_x>0){
			self->entity->v_x = self->entity->v_max;
		}
		else{
			self->entity->v_x = -self->entity->v_max;
		}
	}
	//y-direction
	if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
		self->entity->v_y += self->entity->a_y;
	}
	else if ( abs(self->entity->v_y)<= self->entity->v_max){
		if(self->entity->v_y>0){
			self->entity->v_y = self->entity->v_max;
		}
		else{
			self->entity->v_y = -self->entity->v_max;
		}
	}

	//Apply Friction to velocity
	//x-direction.
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


}

void enemyShooterThink(Object self, Environment mainEnv){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}
//Since the shooter enemy can fire a weapon, the energy attribute must be usable.
	if(self->entity->energy <self->entity->maxEnergy){
		self->entity->energy+= 1;
	}
	else{
		self->entity->energy = self->entity->maxEnergy;
	}
	//Shoot at a random time. The probability is inversly proportional to param1.
	if(!(random()%(self->entity->param1+1))){
		self->entity->weaponA->fire(self->entity->weaponA, self, mainEnv);
	}
	/**Apply acceleration to velocity
	 * Velocity in each direction cannot exceed maximum velocity.
	 */
	//x-direction
	if( abs(self->entity->v_x + self->entity->a_x) < self->entity->v_max){
		self->entity->v_x += self->entity->a_x;
	}
	else if ( abs(self->entity->v_x)<= self->entity->v_max){
		if(self->entity->v_x>0){
			self->entity->v_x = self->entity->v_max;
		}
		else{
			self->entity->v_x = -self->entity->v_max;
		}
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


}


uint8_t enemyCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	//other == 0 represents a collision with the screen border.
	if(other){
		if(other->type ==PLAYER){
			//Decrease own health by player armor value.
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

			//The projectiles armor is equal to its damage.
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
			//If the enemy collided with something else:
			if(iter){
				return 1;
			}
			else{
				return other->collide(other,self,cType,1);
			}
		}
	}
	else{
		//Killed by Border will cause the main loop to spawn another enemy in its place.
		if(cType == LEFTC){
			self->killedBy =BORDER;
		}
		else{
			//Causes enemies to rebound of the edges, however drag will be applied.
			rebound(self,0, cType);
			drag(self, 1);
		}
		return 1;
	}

}

Object getEnemyByType( uint8_t type, uint8_t* sprite,uint8_t health,uint8_t armor,int8_t speed, uint8_t y, uint8_t param){
	Object enemy =0;
	if(type == FLOATER){
		enemy = newEnemyFloater(sprite, y);

	}
	else if(type == GLIDER){
		enemy = newEnemyGlider(sprite,y, param);
	}
	else if(type == TRACKER){
		enemy = newEnemyTracker(sprite,y);
	}
	else if(type == SHOOTER){
		enemy = newEnemyShooter(sprite,y, param);
	}
	else{
		return 0;
	}
	enemy->setXY(enemy, MAXX, enemy->y);
	enemy->entity->health = health;
	enemy->entity->maxHealth = health;
	enemy->entity->armor = armor;
	enemy->entity->a_x = -speed;
	enemy->entity->v_x = -speed;
	enemy->entity->v_max = speed;
	return enemy;


}

