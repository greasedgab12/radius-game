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
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"
#include "sprite.h"


#include "entities/enemy.h"
#include "entities/bullet.h"
#include "entities/player.h"


EnemyEnv newEnemyEnv(){

    EnemyEnv self = (EnemyEnv)malloc(sizeof(struct EnemyEnv_Struct));
    self->health = 5;
    self->armor = 1;

    self->acceleration =0;
    self->v_max = 20 + FRICTION;
    self->a_x = -3;
    self->a_y = 0;
    self->v_x = 0;
    self->v_y = 0;
    self->s_x = 0;
    self->s_y = 0;

    self->state =0;
    self->param1 =0;
    self->param2 =0;

    self->v_time = 0;
    self->v_delay =2;// 25;

    self->rof_time =0;
    self->rof_delay = 30;

    return self;
}

Object newEnemy(uint8_t x, uint8_t y){
	Object self = newObject(x,y,8,8,enemy_vessel_0);
	self->type = ENEMY;
	self->objectEnv = newEnemyEnv();
	self->think = &enemyThink;
	self->collide =&enemyCollide;

	return self;


}

Object newEnemyGlider(uint8_t y, uint8_t f){
	Object self = newEnemy(MAXX-8,y);
	self->think = &enemyGliderThink;

	EnemyEnv env = (EnemyEnv) self->objectEnv;
	env->param1 = f;
	return self;


}

void enemyGliderThink(Object self, Environment mainEnv){
	EnemyEnv env = (EnemyEnv)self->objectEnv;
	//Movement
	if(!self->isAlive){
		return;
	}
	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){

		//Update time variable to correctly acquire time passed.
		env->v_time = mainEnv->time;

		/**Apply acceleration to velocity
		 * Velocity in each direction cannot exceed maximum velocity.
		 */
		//x-direction
		if(env->state<env->param1){
			env->state++;
		}
		else{
			env->state =0;
		}
		/**
		env->a_y += env->a_y<10? (-1)*(env->state/(env->param1/2)*env->state*10/env->param1):0;
		**/
		if(env->state < env->param1/2){

			env->a_y =1 + FRICTION;
		}
		else{
			env->a_y =-1 -FRICTION;
		}
		printN(env->state, 0 ,4);





		if( abs(env->v_x + env->a_x) < env->v_max){
			env->v_x += env->a_x;
		}
		//y-direction
		if( abs(env->v_y + env->a_y) < env->v_max){
			env->v_y += env->a_y;
		}

		//Apply Friction to velocity
		//x-direction
		if(env->v_x > FRICTION){
			env->v_x -=FRICTION;
		}
		else if( env->v_x < -FRICTION){
			env->v_x +=FRICTION;
		}
		else{
			env->v_x = 0;
		}
		//y-direction
		if(env->v_y > FRICTION){
			env->v_y -=FRICTION;
		}
		else if( env->v_y < -FRICTION){
			env->v_y +=FRICTION;
		}
		else{
			env->v_y = 0;
		}
		//Apply velocitiy to position.
		moveObject(self, mainEnv,(env->s_x+env->v_x)/10,(env->s_y+env->v_y)/10);
		//Add the remainder to the next step:
		env->s_x = (env->s_x+env->v_x)%10;
		env->s_y = (env->s_y+env->v_y)%10;

	}

}


void enemyThink(Object self, Environment mainEnv){
	//Cast the void pointer to PlayerEnv.
	EnemyEnv env = (EnemyEnv)self->objectEnv;
	//Movement
	if(!self->isAlive){
		return;
	}

	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){

		//Update time variable to correctly acquire time passed.
		env->v_time = mainEnv->time;
		/**Apply acceleration to velocity
		 * Velocity in each direction cannot exceed maximum velocity.
		 */
		//x-direction
		printN(env->v_x,0,0);
		printN(env->v_y,0,2);
		printN(env->a_x,0,4);
		if( abs(env->v_x + env->a_x) < env->v_max){
			env->v_x += env->a_x;
		}
		//y-direction
		if( abs(env->v_y + env->a_y) < env->v_max){
			env->v_y += env->a_y;
		}

		//Apply Friction to velocity
		//x-direction
		if(env->v_x > FRICTION){
			env->v_x -=FRICTION;
		}
		else if( env->v_x < -FRICTION){
			env->v_x +=FRICTION;
		}
		else{
			env->v_x = 0;
		}
		//y-direction
		if(env->v_y > FRICTION){
			env->v_y -=FRICTION;
		}
		else if( env->v_y < -FRICTION){
			env->v_y +=FRICTION;
		}
		else{
			env->v_y = 0;
		}
		//Apply velocitiy to position.
		moveObject(self, mainEnv,(env->s_x+env->v_x)/10,(env->s_y+env->v_y)/10);
		//Add the remainder to the next step:
		env->s_x = (env->s_x+env->v_x)%10;
		env->s_y = (env->s_y+env->v_y)%10;

	}
}

uint8_t enemyCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	EnemyEnv selfEnv = (EnemyEnv)self->objectEnv;
	if(other){
		if(other->type ==PLAYER){
			PlayerEnv otherEnv = (PlayerEnv) other->objectEnv;

			if(selfEnv->health<otherEnv->armor){
				selfEnv->health = 0;
			}
			else{
				selfEnv->health -= otherEnv->armor;
			}
			if(selfEnv->health ==0){
				self->isAlive =0;
			}
			if(iter){
				return 1;
			}
			else{
				return other->collide(other, self, cType, 0);
			}
		}
		else if( other->type == PLAYER_PROJECTILE){
			BulletEnv otherEnv = (BulletEnv)other->objectEnv;
			rebound(self,other,cType);


			if(selfEnv->health<otherEnv->damage){
				selfEnv->health = 0;
			}
			else{
				selfEnv->health -= otherEnv->damage;
			}
			if(selfEnv->health ==0){
				self->isAlive =0;
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
			self->isAlive =0;
		}
		else{
			rebound(self,0, cType);
			drag(self, 1);
		}
		return 1;
	}

}

