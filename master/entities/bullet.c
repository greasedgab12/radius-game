/*
 * bullet.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */
#include <entities/bullet.h>
#include "sprite.h"
#include <stdlib.h>


BulletEnv newBulletEnv(int8_t v_x, int8_t v_y, uint8_t damage){
	BulletEnv self = (BulletEnv)malloc(sizeof(struct BulletEnv_Struct));
	self->damage =damage;
	self->v_x =v_x;
	self->v_y =v_y;

	self->s_x =0;
	self->s_y =0;

	self->v_delay = 2;
	self->v_time = 0;
	return self;
}


Object newBullet(uint8_t x, uint8_t y, int8_t v_x, int8_t v_y, uint8_t damage){
	Object self = newObject(x,y,4,2,projectile_1);
	self->objectEnv = newBulletEnv(v_x, v_y, damage);
	self->think = &bulletThink;
	self->collide = &bulletCollide;
	return self;
}

void bulletThink(Object self, Environment mainEnv){
	BulletEnv env = (BulletEnv)self->objectEnv;

	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){
		int8_t vx = (env->s_x+env->v_x)/10;
		int8_t vy = (env->s_y+env->v_y)/10;
		//Add the remainder to the velocity
		env->s_x= (env->s_x+env->v_x)%10;
		env->s_y= (env->s_y+env->v_y)%10;

		env->v_time = mainEnv->time;
		moveObject(self, mainEnv, vx, vy);

	}

}

uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	if(other){
		if(other->type == PLAYER){
			if(self->type==PLAYER_PROJECTILE){
				return 0;
			}
			else{
				if(iter){
					return 1;
				}
				else{
					return other->collide(other, self, cType, iter);
				}
			}
		}
		else if(other->type == ENEMY){
			if(self->type==ENEMY_PROJECTILE){
				return 0;
			}
			else{
				drag(other,5);
				self->isAlive =0;
				print("huh",0,4);
				return iter?1:other->collide(other,self,cType, iter++);
			}
		}
		else if(other->type == PLAYER_PROJECTILE){
			return 0;
		}
		else if(other->type == ENEMY_PROJECTILE){
			return 0;
		}
		else{
			self->isAlive =0;
			return 1;
		}
	}
	else{
		self->isAlive =0;
		return 1;
	}
}
