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

	int8_t s_x, s_y;
	s_x = self->entity->v_x%10;
	s_y = self->entity->v_y%10;

	if(self->entity->v_y > 1){
		self->entity->v_y -=1;
	}
	else if( self->entity->v_y < -1){
		self->entity->v_y +=1;
	}
	else{
		self->entity->v_y = 0;
	}

	moveObject(self, mainEnv, (self->entity->v_x + s_x)/10, (self->entity->v_y+s_y)/10);


}

void bulletThink(Object self, Environment mainEnv){
	int8_t s_x, s_y;
	s_x = self->entity->v_x%10;
	s_y = self->entity->v_y%10;
	moveObject(self, mainEnv, (self->entity->v_x+s_x)/10, (self->entity->v_y+s_y)/10);

}

void missileThink(Object self, Environment mainEnv){

	uint8_t i;
	Object target=0;

	for(i=0; i<1+MAXENEMIES; i++){
		if(mainEnv->objectList[i]->activeState == ACTIVE){
			if(self->type == PLAYER_PROJECTILE && mainEnv->objectList[i]->type == ENEMY){
				target = mainEnv->objectList[i];
				break;
			}
			else if(self->type == ENEMY_PROJECTILE && mainEnv->objectList[i]->type == PLAYER){
				target = mainEnv->objectList[i];
				break;
			}
		}
	}
	int8_t s_x, s_y;
	s_x = self->entity->v_x%10;
	s_y = self->entity->v_y%10;

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
	moveObject(self, mainEnv, (self->entity->v_x+s_x)/10, (self->entity->v_y+s_y)/10);

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


void newProjectile(Object self, uint8_t projectileType){
	if(projectileType == BULLET){
		newObject(self,0,0,5,2,bullet_sprite);
		newEntity(self->entity);
		self->think =&bulletThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == MISSILE){
		newObject(self,0,0,8,4,missile_sprite);
		newEntity(self->entity);
		self->think =&missileThink;
		self->collide = &bulletCollide;
	}
	else if(projectileType == DISC){
		newObject(self,0,0,6,6,disc_sprite);
		newEntity(self->entity);
		self->think =&discThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == BULLETHEAVY){
		newObject(self,0,0,8,5,bulletHeavy_sprite);
		newEntity(self->entity);
		self->think =&bulletThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == SHOTUPPER){
		newObject(self,0,0,4,4,shotupper_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == SHOTLOWER){
		newObject(self,0,0,4,4,shotlower_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == SHOTMIDDLE){
		newObject(self,0,0,4,4,shotmiddle_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == BALL){
		newObject(self,0,0,8,8,ball_sprite);
		newEntity(self->entity);
		self->think =&bulletThink;
		self->collide = &ballCollide;

	}
	else{
		newObject(self,0,0,4,4,bulletEnemy_sprite);
		newEntity(self->entity);
		self->think =&bulletThink;
		self->collide = &bulletCollide;

	}
}
