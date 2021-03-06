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

#include "structure.h"
#include "defines.h"
#include "environment.h"

#include "object.h"

#include "entity.h"
#include "general.h"
#include "projectile.h"
#include "enemy.h"

#include "weapon.h"


#include "display.h"
#include "char.h"
#include "sprites.h"
#include "uart.h"




void newEnemy(Object self ,uint8_t * sprite,uint8_t x, uint8_t y){
	newObject(self,x,y,9,9,sprite);
	self->lx = self->slx;
	self->ly = self->sly*4;
	self->type = ENEMY;
	newEntity(self->entity);
	self->think = &enemyThink;
	self->collide =&enemyCollide;

}

void  newEnemyFloater(Object self,uint8_t * sprite,uint8_t y){
	newEnemy(self,sprite,0,y);
	self->setXY(self,MAXX -self->lx, y);
}

void newEnemyGlider(Object self,uint8_t * sprite,uint8_t y, uint8_t f){
	newEnemy(self,sprite,0,y);
	self->think = &enemyGliderThink;
	self->setXY(self,MAXX -self->lx, y);
	self->entity->param1 = f;
}

void newEnemyTracker(Object self,uint8_t * sprite,uint8_t y){
	newEnemy(self,sprite,0,y);
	self->think = &enemyTrackerThink;
	self->setXY(self,MAXX -self->lx, y);
}

void newEnemyShooter(Object self,uint8_t * sprite,uint8_t y, uint8_t invProbability){
	newEnemy(self,sprite,0,y);
	self->think = &enemyShooterThink;
	self->entity->param1 = invProbability;
	self->setXY(self,MAXX -self->lx, y);
}

void enemyThink(Object self, Environment env){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}

	//Apply acceleration to velocity.
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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Save the remainder for the next movement.
	self->entity->s_x = self->entity->v_x%10;
	self->entity->s_y = self->entity->v_y%10;


}


void enemyGliderThink(Object self, Environment env){
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



	//Apply acceleration to velocity.
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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Save the remainder for the next movement.
	self->entity->s_x = self->entity->v_x%10;
	self->entity->s_y = self->entity->v_y%10;

}

void enemyTrackerThink(Object self, Environment env){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}

	//Set the player as the acceleration target.
	Object target=env->player;


	//Calculate acceleration based on target position.
	//The tracker enemy can only accelerate in the y-axis.
	//x-axis movement is possible but rather unfair.
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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
		//Save the remainder for the next movement.
	self->entity->s_x = self->entity->v_x%10;
	self->entity->s_y = self->entity->v_y%10;


}

void enemyShooterThink(Object self, Environment env){
	//Killed objects don't think.
	if(self->killedBy!=0){
		return;
	}
	//Shoot at a random time. The probability is inversly proportional to param1.
	if(!(random()%(self->entity->param1+1))){
		//Get a projectile slot from the environment.
		Object slot = getProjectileSlot(env);
		if(slot){

		uart_putc('a');
		newProjectile(slot,BULLETENEMY);

		slot->type = ENEMY_PROJECTILE;
		slot->setXY(slot,self->x, self->y + self->ly/2 - slot->ly/2);
		slot->entity->v_x = self->entity->v_x + -8;
		slot->entity->a_x = -2;
		slot->entity->v_max = 10;

		slot->entity->armor = 1 + self->entity->armor/3;
		slot->entity->health = 1;
		slot->activeState = ACTIVE;
		slot->drawState = NOTDRAWN;
		}

	}

	/**Apply acceleration to velocity
	 * Velocity in each direction cannot exceed maximum velocity.
	 */
	//Only accelerate in the x-direction
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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
		//Save the remainder for the next movement.
	self->entity->s_x = self->entity->v_x%10;
	self->entity->s_y = self->entity->v_y%10;

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
				//Call the collision method the the player.
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
				//Call the collision method of the projectile.
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

void getEnemyByType(Object enemy, uint8_t type, uint8_t* sprite,uint8_t health,uint8_t armor,int8_t speed, uint8_t y, uint8_t param){

	if(type == FLOATER){
		newEnemyFloater(enemy,sprite, y);
	}
	else if(type == GLIDER){
		//param acts as phaselength variable.
		newEnemyGlider(enemy,sprite,y, param);
	}
	else if(type == TRACKER){
		newEnemyTracker(enemy,sprite,y);
	}
	else if(type == SHOOTER){
		//param controls shot frequency
		newEnemyShooter(enemy,sprite,y, param);
	}
	//Set position to the rightmost.
	enemy->setXY(enemy, MAXX, enemy->y);
	enemy->entity->health = health;
	enemy->entity->maxHealth = health;
	enemy->entity->armor = armor;
	enemy->entity->a_x = -speed - FRICTION;
	enemy->entity->v_x = -speed - FRICTION;
	enemy->entity->v_max = speed;



}

