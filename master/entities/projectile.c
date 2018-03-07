/*
 * bullet.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */
#include <entities/projectile.h>
#include "entity.h"
#include "sprite.h"

#include <stdlib.h>





void bulletThink(Object self, Environment mainEnv){

	if(((uint16_t) mainEnv->time) >= self->entity->v_time + self->entity->v_delay){
		int8_t vx = (self->entity->s_x+self->entity->v_x)/10;
		int8_t vy = (self->entity->s_y+self->entity->v_y)/10;
		//Add the remainder to the velocity
		self->entity->s_x= (self->entity->s_x+self->entity->v_x)%10;
		self->entity->s_y= (self->entity->s_y+self->entity->v_y)%10;

		self->entity->v_time = mainEnv->time;
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

Object newProjectile(uint8_t projectileType){
	if(projectileType == BULLET){
		Object self = newObject(0,0,3,2,projectile_1);
		self->entity = newEntity();
		self->think =&bulletThink;
		self->collide = &bulletCollide;
		return self;

	}
	return 0;
}
