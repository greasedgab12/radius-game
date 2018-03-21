/*
 * bullet.c
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

#include "sprites.h"





void newProjectile(Object self, uint8_t projectileType){
	//Create projectile from projectile type. See defines.h.
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
		self->entity->param1 =0;
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
	else if(projectileType == BULLETENEMY){
		newObject(self,0,0,4,4,bulletEnemy_sprite);
		newEntity(self->entity);
		self->think =&bulletThink;
		self->collide = &bulletCollide;

	}
	else if(projectileType == LASER0){
		newObject(self,0,0,1,1,laser0_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &laserCollide;

	}
	else if(projectileType == LASER1){
		newObject(self,0,0,1,2,laser1_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &laserCollide;

	}
	else if(projectileType == LASER2){
		newObject(self,0,0,1,3,laser2_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &laserCollide;

	}
	else if(projectileType == LASER3){
		newObject(self,0,0,1,4,laser3_sprite);
		newEntity(self->entity);
		self->think =&shotThink;
		self->collide = &laserCollide;

	}
	//Activate object.
	self->activeState = ACTIVE;
}


void bulletThink(Object self, Environment env){
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Add the remainder of the movement to the next move.
	self->entity->s_x += self->entity->v_x%10;
	self->entity->s_y += self->entity->v_y%10;
	self->entity->s_x = self->entity->s_x%10;
	self->entity->s_y = self->entity->s_y%10;

}

void discThink(Object self, Environment env){
	//Deccelerate y-velocity every 3 method calls.
	self->entity->param1++;
	if(self->entity->param1>=2){
		self->entity->param1 =0;
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


	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Add the remainder of the movement to the next move.
	self->entity->s_x += self->entity->v_x%10;
	self->entity->s_y += self->entity->v_y%10;
	self->entity->s_x = self->entity->s_x%10;
	self->entity->s_y = self->entity->s_y%10;


}

void missileThink(Object self, Environment env){

	uint8_t i;
	Object target=0;
	//Search for oldest active enemy in game environment.
	for(i=0; i<1+MAXENEMIES; i++){
		if(env->objectList[i]->activeState == ACTIVE){
			if(self->type == PLAYER_PROJECTILE && env->objectList[i]->type == ENEMY){
				target = env->objectList[i];
				break;
			}
			else if(self->type == ENEMY_PROJECTILE && env->objectList[i]->type == PLAYER){
				target = env->objectList[i];
				break;
			}
		}
	}
	//Accelerate to the y-coordinate of the target.
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
	moveObject(self, env,(self->entity->v_x+self->entity->s_x)/10,(self->entity->v_y+self->entity->s_y)/10);
	//Add the remainder of the movement to the next move.
	self->entity->s_x += self->entity->v_x%10;
	self->entity->s_y += self->entity->v_y%10;
	self->entity->s_x = self->entity->s_x%10;
	self->entity->s_y = self->entity->s_y%10;
}

void shotThink(Object self, Environment env){
	//Shots have a limited time to live.
	if(self->entity->health>0){
		self->entity->health--;
	}
	else{
		self->killedBy=PLAYER;
	}
}

uint8_t bulletCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	//If the projectile collides with anything, destroy projectile.
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

uint8_t ballCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	if(other){
		if(other->type == PLAYER){
			if(self->type==PLAYER_PROJECTILE){
				return 0;
			}
			else{
				if(iter){
					//Reduce projectile health.
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
					//Rebound projectile.
					rebound(self,0,cType);
					//Reduce projectile health.
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
					//Reduce projectile health.
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
					//Rebound projectile.
					rebound(self,0,cType);
					//Reduce projectile health.
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
		//Rebound off the screen edges.
		rebound(self,0,cType);
		//Reduce projectile health.
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



uint8_t laserCollide(Object self, Object other,uint8_t cType,uint8_t iter){
	//Laser Collisions reduce the projectiles health, but leaves its position and velocity unchanged.
	if(other){
		if(other->type == PLAYER){
			if(self->type==PLAYER_PROJECTILE){
				return 0;
			}
			else{
				if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = PLAYER;
					}
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
				if(self->entity->health >0){
						self->entity->health--;
					}
					else{
						self->entity->health =0;
						self->killedBy = ENEMY;
					}
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





