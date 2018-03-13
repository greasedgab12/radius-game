/*
 * bullet.c
 *
 *  Created on: Mar 2, 2018
 *      Author: root
 */
#include <entities/projectile.h>
#include "entity.h"
#include "sprites.h"

#include <stdlib.h>





void discThink(Object self, Environment mainEnv){

	moveObject(self, mainEnv, (self->entity->v_x)/10, (self->entity->v_y)/10);

	self->entity->param1 = self->entity->param1<3?self->entity->param1+1:0;
	//y-direction
	if(self->entity->param1 ==0){
		if(self->entity->v_y > 1){
			self->entity->v_y -=1;
		}
		else if( self->entity->v_y < -1){
			self->entity->v_y +=1;
		}
		else{
			self->entity->v_y = 0;
		}
	}
}

void bulletThink(Object self, Environment mainEnv){

		moveObject(self, mainEnv, self->entity->v_x/10, self->entity->v_y/10);

}

void missileThink(Object self, Environment mainEnv){

	uint8_t i;
	Object target=0;

	for(i=1; i<mainEnv->oPos; i++){
		if(self->type == PLAYER_PROJECTILE && mainEnv->objectList[i]->type == ENEMY){
			target = mainEnv->objectList[i];
			break;
		}
		else if(self->type == ENEMY_PROJECTILE && mainEnv->objectList[i]->type == PLAYER){
			target = mainEnv->objectList[i];
			break;
		}
	}
	if(target){
		if(self->y < target->y){
			self->entity->a_y = (self->entity->v_max/5 +FRICTION);
		}
		else if(self->y + self->ly > target->y){
			self->entity->a_y = -(self->entity->v_max/5 +FRICTION);
		}
		else{
			self->entity->a_y =0;
		}
	}

	//y-direction
	if( abs(self->entity->v_y + self->entity->a_y) < self->entity->v_max){
		self->entity->v_y += self->entity->a_y;
	}
	moveObject(self, mainEnv, self->entity->v_x, self->entity->v_y);

}

uint8_t ballCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	if(other){
		if(other->type == PLAYER){
			if(self->type==PLAYER_PROJECTILE){
				return 0;
			}
			else{
				if(iter){
					if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = PLAYER;
					}
					return 1;
				}
				else{
					rebound(self,0,cType);
					if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = PLAYER;
					}
					return other->collide(other, self, cType, 1);
				}
			}
		}
		else if(other->type == ENEMY){
			if(self->type==PLAYER_PROJECTILE){
				if(iter){

					if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = ENEMY;
					}
					return 1;
				}
				else{
					rebound(self,0,cType);
					if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = ENEMY;
					}
					return other->collide(other, self, cType, 1);
				}
			}
			else{
				return 0;
			}
		}
		else if(other->type == PLAYER_PROJECTILE){
			return 0;
		}
		else if(other->type == ENEMY_PROJECTILE){
			return 0;
		}
		else{
			self->killedBy =OBSTACLE;
			return 1;
		}
	}
	else{
		rebound(self,0,cType);
		if(self->entity->health >0){
			self->entity->health--;
		}
		else{
			self->entity->health =0;
			self->killedBy = BORDER;
		}
		return 1;
	}

}

uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	if(other){
		if(other->type == PLAYER){
			if(self->type==PLAYER_PROJECTILE){
				return 0;
			}
			else{
				self->killedBy=PLAYER;
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
				self->killedBy =ENEMY;
				if(iter){

					return 1;
				}
				else{

					return other->collide(other, self, cType, iter);
				}
			}
		}
		else if(other->type == PLAYER_PROJECTILE){
			return 0;
		}
		else if(other->type == ENEMY_PROJECTILE){
			return 0;
		}
		else{
			self->killedBy =OBSTACLE;
			return 1;
		}
	}
	else{
		self->killedBy =BORDER;
		return 1;
	}
}

void shotThink(Object self, Environment mainEnv){

	if(self->entity->health>0){
		self->entity->health--;
	}
	else{
		self->killedBy=PLAYER;
	}
}


Object newProjectile(uint8_t projectileType){
	if(projectileType == BULLET){
		Object self = newObject(0,0,5,2,bullet_sprite);
		self->entity = newEntity();
		self->think =&bulletThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == MISSILE){
		Object self = newObject(0,0,8,4,missile_sprite);
		self->entity = newEntity();
		self->think =&missileThink;
		self->collide = &bulletCollide;
		return self;
	}
	else if(projectileType == DISC){
		Object self = newObject(0,0,6,6,disc_sprite);
		self->entity = newEntity();
		self->think =&discThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == BULLETHEAVY){
		Object self = newObject(0,0,8,5,bulletHeavy_sprite);
		self->entity = newEntity();
		self->think =&bulletThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == SHOTUPPER){
		Object self = newObject(0,0,4,4,shotupper_sprite);
		self->entity = newEntity();
		self->think =&shotThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == SHOTLOWER){
		Object self = newObject(0,0,4,4,shotlower_sprite);
		self->entity = newEntity();
		self->think =&shotThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == SHOTMIDDLE){
		Object self = newObject(0,0,4,4,shotmiddle_sprite);
		self->entity = newEntity();
		self->think =&shotThink;
		self->collide = &bulletCollide;
		return self;

	}
	else if(projectileType == BALL){
		Object self = newObject(0,0,8,8,ball_sprite);
		self->entity = newEntity();
		self->think =&bulletThink;
		self->collide = &ballCollide;
		return self;

	}
	return 0;
}
