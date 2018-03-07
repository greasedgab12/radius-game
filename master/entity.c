/*
 * entity.c
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */

#include <stdlib.h>
#include "defines.h"
#include "entity.h"
#include "structure.h"

Entity newEntity(){

    Entity self = (Entity)malloc(sizeof(struct Entity_Struct));
    self->health = 5;
    self->armor = 1;

    self->acceleration =0;
    self->v_max = 20 + FRICTION;
    self->a_x = -3;
    self->a_y = 0;
    self->v_x = 0;
    self->v_y = 0;
    self->s_x = 0;
    self->s_y = 0;

    self->state =0;
    self->param1 =0;
    self->param2 =0;

    self->weaponA =0;
    self->weaponB =0;

    self->v_time = 0;
    self->v_delay =2;// 25;


    return self;
}

void releaseEntity(Entity instance){
	if(instance->weaponA){
		free(instance->weaponA);
	}
	if(instance->weaponB){
		free(instance->weaponB);
	}
	free(instance);
}
