/*
 * entities.c
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */
#include "entities.h"
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include <stdlib.h>

const uint8_t player[] = {4,8,0xff,0xff,0x0c,0x30,0x30,0x0c,0xc0,0x03};

PlayerEnv newPlayerEnv(){

    PlayerEnv self = (PlayerEnv)malloc(sizeof(struct PlayerEnv_Struct));
    self->health = 5;
    self->energy = 0;
    self->defense = 0;

    self->acceleration = 7;
    self->v_max = 50;
    self->v_x = 0;
    self->v_y = 0;

    self->v_time = 0;
    self->v_delay = 25;

    self->rof_time =0;
    self->rof_delay = 50;

    return self;
}

Object newPlayer(uint8_t x, uint8_t y){
	Object self = newObject(x,y,player);
	self->type = PLAYER;
	self->objectEnv = newPlayerEnv();
	self->think = &playerThink;
	self->collide =&playerCollide;

	return self;


}

void playerThink(Object self, Environment mainEnv){
	PlayerEnv env = (PlayerEnv)self->objectEnv;
	//Movement
	int8_t a_x = 0, a_y = 0;
	char text[11];

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
	if(((uint16_t) mainEnv->time) >= env->v_time + env->v_delay){
		printN(env->v_x , 80,0);
		printN(env->v_y , 80,2);

		env->v_time = mainEnv->time;
		itoa(env->v_time, text, 10);
		print(text,0,0);
		//Apply acceleration to velocity
		if( abs(env->v_x + a_x) < env->v_max){
			env->v_x += a_x;
		}
		else{
			env->v_x = env->v_x>0? env->v_max: -env->v_max;
		}
		if( abs(env->v_y + a_y) < env->v_max){
			env->v_y += a_y;
		}
		else{
			env->v_y = env->v_y>0? env->v_max: -env->v_max;
		}
		//Apply Friction to velocity
		if(env->v_x > FRICTION){
			env->v_x -=FRICTION;
		}
		else if( env->v_x < -FRICTION){
			env->v_x +=FRICTION;
		}
		else{
			env->v_x = 0;
		}
		if(env->v_y > FRICTION){
			env->v_y -=FRICTION;
		}
		else if( env->v_y < -FRICTION){
			env->v_y +=FRICTION;
		}
		else{
			env->v_y = 0;
		}

		if(env->v_x <0 && self->x<abs(env->v_x/10) && env->v_y <0 && self->y<abs(env->v_y/10)){
			moveObject(self, mainEnv, 0, 0);
			env->v_x =0;
			env->v_y =0;
		}
		else if( !(env->v_x <0 && self->x<abs(env->v_x/10)) && (env->v_y <0 && self->y<abs(env->v_y/10))){
			moveObject(self, mainEnv, self->x+env->v_x/10, 0);
			env->v_y =0;

		}
		else if((env->v_x <0 && self->x<abs(env->v_x/10)) && !(env->v_y <0 && self->y<abs(env->v_y/10))){
			moveObject(self, mainEnv, 0, self->y+env->v_y/10);
			env->v_x =0;
		}
		else{
		moveObject(self, mainEnv, self->x+env->v_x/10, self->y+env->v_y/10);
		}
	}
}

uint8_t playerCollide(Object self, Object other){
	PlayerEnv env = (PlayerEnv) self->objectEnv;

	if(other->type == ENEMY){
		env->health = env->health>0? env->health-1: 0;
		return 1;
	}
	else{
		return other->collide(other, self);
	}
}

void moveOnButtonThink(Object self, Environment mainEnv){

	if(mainEnv->buttons & M_U){
		self->move(self,mainEnv, self->x, self->y-1);
	}
	else if(mainEnv->buttons & M_D){
		self->move(self,mainEnv, self->x, self->y+1);
	}
	else if(mainEnv->buttons & M_L){
		self->move(self,mainEnv, self->x-1, self->y);
	}
	else if(mainEnv->buttons & M_R){
		self->move(self, mainEnv, self->x+1, self->y);
	}

}

void noOp(){;}

uint8_t noCollide(Object self, Object other){
	return 0;
}

uint8_t simpleCollide(Object self, Object other){
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
