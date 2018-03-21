/*
 * entities.h
 *
 *  Created on: Feb 28, 2018
 *      Author: root
 */

#ifndef MASTER_ENTITIES_H_
#define MASTER_ENTITIES_H_
#include "general.h"
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "object.h"
#include "display.h"
#include "char.h"
#include "environment.h"


//Dummy think routine. Does nothing.
void noOp();

/**noCollide: Cause involved objects not to collide.
 * Object self: Object the collide method is called from.
 * Object other: Other object involved in the collision.
 * uint8_t cType: Collision type. See defines.h
 * uint8_t iter: Call iteration.
 * return unit8_t: 0: no collision, 1: collision.
 */
uint8_t noCollide(Object self, Object other,uint8_t cType, uint8_t iter);

/**simpleCollide: Cause involved objects to collide and rebound.
 * Object self: Object the collide method is called from.
 * Object other: Other object involved in the collision.
 * uint8_t cType: Collision type. See defines.h
 * uint8_t iter: Call iteration.
 * return unit8_t: 0: no collision, 1: collision.
 */
uint8_t simpleCollide(Object self, Object other,uint8_t cType, uint8_t iter);

/**rebound: Cause two Objects to rebound off one another.
 * Object self: First object.
 * Object other: Second object.
 * uint8_t cType: Collision type. See defines.h
 */
void rebound(Object self,Object other, uint8_t cType);

/**drag: Apply drag to an object.
 * Object self: Object to be deccelerated.
 * uint8_t cff: Flat decceleration value.
 */
void drag(Object self, uint8_t cff);



#endif /* MASTER_ENTITIES_H_ */
