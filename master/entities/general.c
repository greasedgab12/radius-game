/*
 * entities.c
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */
#include <entities/general.h>
#include <entities/projectile.h>
#include <entity.h>
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"


#include "entities/player.h"





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
		ux = abs(self->entity->v_x + other->entity->v_x)/2;
		uy = abs(self->entity->v_y + other->entity->v_y)/2;

		ux = ux?ux+20:0;
		uy = uy?uy+20:0;

		if(self->x < other->x){
			self->entity->v_x = -ux/2;
			other->entity->v_x = ux;
		}
		else{
			self->entity->v_x = ux/2;
			other->entity->v_x = -ux;
		}
		if(self->y<other->y){
			self->entity->v_y = -uy/2;
			other->entity->v_y = uy;
		}
		else{
			self->entity->v_y = uy/2;
			other->entity->v_y = -uy;
		}
	}
	else if(other ==0){
		if(cType == LEFTC || cType ==RIGHTC){
			self->entity->v_x = -(self->entity->v_x);
		}
		else if(cType == UPPERC || cType == LOWERC){
			self->entity->v_y = -(self->entity->v_y);
		}
	}

	else{

		ux = abs(self->entity->v_x + other->entity->v_x)/2;
		uy = abs(self->entity->v_y + other->entity->v_y)/2;

		ux = ux?ux+20:0;
		uy = uy?uy+20:0;

		if(self->x < other->x){
			self->entity->v_x = -ux;
			other->entity->v_x = ux;
		}
		else{
			self->entity->v_x = ux;
			other->entity->v_x = -ux;
		}
		if(self->y<other->y){
			self->entity->v_y = -uy;
			other->entity->v_y = uy;
		}
		else{
			self->entity->v_y = uy;
			other->entity->v_y = -uy;
		}

	}

}

void drag(Object self, uint8_t cff){
	if(self->type){
		if(self->type != PLAYER_PROJECTILE && self->type!=ENEMY_PROJECTILE){
				self->entity->v_x += self->entity->v_x<0? cff: -cff;
				self->entity->v_y += self->entity->v_y<0? cff: -cff;
			}
		else{

				self->entity->v_x += self->entity->v_x<0? cff: -cff;
				self->entity->v_y += self->entity->v_y<0? cff: -cff;
		}
	}
}

