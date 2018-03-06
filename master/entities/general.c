/*
 * entities.c
 *
 *  Created on: Feb 28, 2018
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


#include "entities/player.h"
#include "entities/enemy.h"
#include "entities/bullet.h"





void moveOnButton(Object self, Environment mainEnv){

	if(mainEnv->buttons & M_U){
		moveObject(self,mainEnv, 0, -1);
	}
	else if(mainEnv->buttons & M_D){
		moveObject(self,mainEnv, 0, 1);
	}
	else if(mainEnv->buttons & M_L){
		moveObject(self,mainEnv, -1, 0);
	}
	else if(mainEnv->buttons & M_R){
		moveObject(self, mainEnv, 1, 0);
	}

}

void noOp(){;}

uint8_t noCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	return 0;
}

uint8_t simpleCollide(Object self, Object other,uint8_t cType, uint8_t iter){
	rebound(other,0,cType);
	return 1;
}

void rebound(Object self,Object other, uint8_t cType){
	int8_t ux,uy;

	if(self->type == PLAYER && other->type == ENEMY){
		PlayerEnv selfEnv = (PlayerEnv) self->objectEnv;
		EnemyEnv otherEnv = (EnemyEnv) other->objectEnv;
		ux = abs(selfEnv->v_x + otherEnv->v_x)/2;
		uy = abs(selfEnv->v_y + otherEnv->v_y)/2;

		ux = ux?ux+20:0;
		uy = uy?uy+20:0;

		if(self->x < other->x){
			selfEnv->v_x = -ux;
			otherEnv->v_x = ux;
		}
		else{
			selfEnv->v_x = ux;
			otherEnv->v_x = -ux;
		}
		if(self->y<other->y){
			selfEnv->v_y = -uy;
			otherEnv->v_y = uy;
		}
		else{
			selfEnv->v_y = uy;
			otherEnv->v_y = -uy;
		}
	}

	else if(self->type == ENEMY && other->type == PLAYER_PROJECTILE){
		EnemyEnv selfEnv = (EnemyEnv) self->objectEnv;
		BulletEnv otherEnv = (BulletEnv) self->objectEnv;
		if(selfEnv->armor + otherEnv->damage==0){
			ux = selfEnv->v_x;
			uy = selfEnv->v_y;
			selfEnv->v_x = otherEnv->v_x;
			selfEnv->v_y = otherEnv->v_y;
		}
		else{
			ux = 2*(selfEnv->v_x*selfEnv->armor + otherEnv->v_x*otherEnv->damage)/(selfEnv->armor + otherEnv->damage);
			uy = 2*(selfEnv->v_y*selfEnv->armor + otherEnv->v_y*otherEnv->damage)/(selfEnv->armor + otherEnv->damage);
			selfEnv->v_x = ux -selfEnv->v_x;
			selfEnv->v_y = uy -selfEnv->v_y;
		}

	}
	else if(self->type == ENEMY && other ==0){
		EnemyEnv env = (EnemyEnv) self->objectEnv;
		if(cType == LEFTC || cType ==RIGHTC){
			env->v_x = -env->v_x;
		}
		else if(cType == UPPERC || cType == LOWERC){
			env->v_y = -env->v_y;
		}
	}
	else if(self->type == PLAYER && other ==0){
		PlayerEnv env = (PlayerEnv) self->objectEnv;
		if(cType == LEFTC || cType ==RIGHTC){
			env->v_x = -env->v_x;
		}
		else if(cType == UPPERC || cType == LOWERC){
			env->v_y = -env->v_y;
		}
	}

}

void drag(Object self, uint8_t cff){
	if(self->type == PLAYER){
			PlayerEnv env = (PlayerEnv) self->objectEnv;
			env->v_x += env->v_x<0? cff: -cff;
			env->v_y += env->v_y<0? cff: -cff;
		}
	else if(self->type == ENEMY){
			EnemyEnv env = (EnemyEnv) self->objectEnv;
			env->v_x += env->v_x<0? cff: -cff;
			env->v_y += env->v_y<0? cff: -cff;
		}
}

