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


#include "entities/player.h"
#include "entities/bullet.h"


PlayerEnv newPlayerEnv(){

    PlayerEnv self = (PlayerEnv)malloc(sizeof(struct PlayerEnv_Struct));
    self->health = 5;
    self->energy = 0;
    self->defense = 0;

    self->acceleration = 1 + FRICTION;
    self->v_max = 50 + FRICTION;
    self->v_x = 0;
    self->v_y = 0;
    self->s_x = 0;
    self->s_y = 0;

    self->v_time = 0;
    self->v_delay =2;// 25;

    self->rof_time =0;
    self->rof_delay = 30;

    return self;
}

Object newPlayer(uint8_t x, uint8_t y){
	Object self = newObject(x,y,8,8,vessel_0);
	self->type = PLAYER;
	self->objectEnv = newPlayerEnv();
	self->think = &playerThink;
	self->collide =&playerCollide;

	return self;


}

void playerThink(Object self, Environment mainEnv){
	//Cast the void pointer to PlayerEnv.
	PlayerEnv env = (PlayerEnv)self->objectEnv;
	//Movement
	int8_t a_x = 0, a_y = 0;
	//Button reactions
	/**D-Pad
	 * Only non opposing directions can be applied to the acceleration.
	**/
	if(mainEnv->buttons & M_U){
		a_y = -env->acceleration;
	}
	else if(mainEnv->buttons & M_D){
		a_y = env->acceleration;
	}

	if(mainEnv->buttons & M_L){
		a_x = -env->acceleration;
	}
	else if(mainEnv->buttons & M_R){
		a_x = env->acceleration;
	}
	/**
	 * A and B buttons.
	 */
	if(mainEnv->buttons & M_A){
		if(((uint16_t) mainEnv->time) >= env->rof_time + env->rof_delay){
			env->rof_time = mainEnv->time;
			Object b0 = newBullet(self->x+self->lx, self->y+3, 25, 0,1 );
			b0->type = PLAYER_PROJECTILE;
			addObject(mainEnv, b0);
		}
	}
	else if(mainEnv->buttons & M_B){

		if(((uint16_t) mainEnv->time) >= env->rof_time + env->rof_delay){
					env->rof_time = mainEnv->time;
					Object b0 = newBullet(self->x+self->lx, self->y, 30, -2,1 );
					b0->type = PLAYER_PROJECTILE;
					Object b1 = newBullet(self->x+self->lx, self->y+4, 30, 0,1 );
					b1->type = PLAYER_PROJECTILE;
					Object b2 = newBullet(self->x+self->lx, self->y+8, 30, 2,1 );
					b2->type = PLAYER_PROJECTILE;
					addObject(mainEnv, b0);
					addObject(mainEnv, b1);
					addObject(mainEnv, b2);
				}
	}

	/**
	 * Calculate movement of the player.
	 */


	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){

		//Update time variable to correctly acquire time passed.
		env->v_time = mainEnv->time;
		/**Apply acceleration to velocity
		 * Velocity in each direction cannot exceed maximum velocity.
		 */
		//x-direction

		if( abs(env->v_x + a_x) < env->v_max){
			env->v_x += a_x;
		}
		else{
			env->v_x = env->v_x>0? env->v_max: -env->v_max;
		}
		//y-direction
		if( abs(env->v_y + a_y) < env->v_max){
			env->v_y += a_y;
		}
		else{
			env->v_y = env->v_y>0? env->v_max: -env->v_max;
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

uint8_t playerCollide(Object self, Object other, uint8_t iter){
	if(other){
		switch(other->type){
		case ENEMY:
			return iter?1:other->collide(other,self,iter++);

		default:
			return iter?0:other->collide(other,self,iter++);
		}
	}
	else{
		return 1;
	}

}
