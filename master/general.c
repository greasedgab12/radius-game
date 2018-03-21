/*
 * entities.c
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */


#include "structure.h"
#include "defines.h"

#include "general.h"



void moveOnButton(Object self, Environment env){

	if(env->buttons & M_U){
		moveObject(self,env, 0, -1);
	}
	else if(env->buttons & M_D){
		moveObject(self,env, 0, 1);
	}
	else if(env->buttons & M_L){
		moveObject(self,env, -1, 0);
	}
	else if(env->buttons & M_R){
		moveObject(self, env, 1, 0);
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
		//Get the average velocity of the two objects.
		ux = abs(self->entity->v_x + other->entity->v_x)/2;
		uy = abs(self->entity->v_y + other->entity->v_y)/2;
		//Add 20 to the averages if not zero.
		ux = ux?ux+20:0;
		uy = uy?uy+20:0;
		//Apply the averages to the velocity of the objects. The Player will only recieve half of it.
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
		//Border collision will simply invert object velocity corresponding to the collision type.
		if(cType == LEFTC || cType ==RIGHTC){
			self->entity->v_x = -(self->entity->v_x);
		}
		else if(cType == UPPERC || cType == LOWERC){
			self->entity->v_y = -(self->entity->v_y);
		}
	}

	else{
		//Other objects will act similar to the player enemy pairing.
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
	//Reduce the object velocity by cff.
	if(self->entity->v_x < -cff){
		self->entity->v_x += cff;
	}
	else if(self->entity->v_x > cff){
		self->entity->v_x -= cff;
	}
	else{
		self->entity->v_x = 0;
	}

	if(self->entity->v_y < -cff){
		self->entity->v_y += cff;
	}
	else if(self->entity->v_y > cff){
		self->entity->v_y -= cff;
	}
	else{
		self->entity->v_y = 0;
	}
}

