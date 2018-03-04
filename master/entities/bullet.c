/*
 * bullet.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */
#include <entities/bullet.h>


const uint8_t bullet[] = {4,4,0x0f,0x0f,0x0f,0x0f};

BulletEnv newBulletEnv(int8_t v_x, int8_t v_y, uint8_t damage){
	BulletEnv self = (BulletEnv)malloc(sizeof(struct BulletEnv_Struct));
	self->damage =damage;
	self->v_x =v_x;
	self->v_y =v_y;
	self->v_delay = 10;
	self->v_time = 0;
	return self;
}


Object newBullet(uint8_t x, uint8_t y, int8_t v_x, int8_t v_y, uint8_t damage){
	Object self = newObject(x,y,4,2,bullet);
	self->objectEnv = newBulletEnv(v_x, v_y, damage);
	self->think = &bulletThink;
	self->collide = &bulletCollide;
	return self;
}

void bulletThink(Object self, Environment mainEnv){
	BulletEnv env = (BulletEnv)self->objectEnv;
	int8_t vx = env->v_x/10;
	int8_t vy = env->v_y/10;

	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){
		env->v_time = mainEnv->time;
		moveObject(self, mainEnv, vx, vy);
	}

}

uint8_t bulletCollide(Object self, Object other,uint8_t iter){
	if(other){
		switch(other->type){
		case(PLAYER):
				if(self->type==PLAYER_PROJECTILE){
					return 0;
				}
				else{
					return iter?0:other->collide(other,self, iter++);
				}
		case(ENEMY):
				if(self->type==ENEMY_PROJECTILE){
					return 0;
				}
				else{
					drag(other,5);
					self->isAlive =0;
					return iter?0:other->collide(other,self, iter++);
				}
		default:
			self->isAlive =0;
			return 1;
		}
	}
	else{
		self->isAlive =0;
		return 1;
	}
}
