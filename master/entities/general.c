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

uint8_t noCollide(Object self, Object other, uint8_t iter){
	return 0;
}

uint8_t simpleCollide(Object self, Object other, uint8_t iter){
	rebound(other, 0);
	return 1;
}

void rebound(Object self, uint8_t cff){
	if(self->type == PLAYER){
		PlayerEnv env = (PlayerEnv) self->objectEnv;
		env->v_x = -env->v_x;
		env->v_y = -env->v_y;
		env->v_x += env->v_x<0? cff: -cff;
		env->v_y += env->v_y<0? cff: -cff;
	}
}

void drag(Object self, uint8_t cff){
	if(self->type == PLAYER){
			PlayerEnv env = (PlayerEnv) self->objectEnv;
			env->v_x += env->v_x<0? cff: -cff;
			env->v_y += env->v_y<0? cff: -cff;
		}
}
