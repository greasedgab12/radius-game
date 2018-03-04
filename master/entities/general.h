/*
 * entities.h
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_H_
#define MASTER_ENTITIES_H_
#include <entities/general.h>
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"



void noOp();

uint8_t noCollide(Object self, Object other, uint8_t iter);

uint8_t simpleCollide(Object self, Object other, uint8_t iter);

void rebound(Object self, uint8_t cff);

void drag(Object self, uint8_t cff);



#endif /* MASTER_ENTITIES_H_ */
