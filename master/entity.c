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

void newEntity(Entity self){

    self->health = 5;
    self->maxHealth =5;
    self->energy = 0;
    self->maxEnergy =0;
    self->armor = 1;

    self->v_max = 20 + FRICTION;

    self->a_x = -3;
    self->a_y = 0;

    self->v_x = 0;
    self->v_y = 0;

    self->state =0;
    self->param1 =0;
}
