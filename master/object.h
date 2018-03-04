#ifndef MASTER_OBJECT_H_
#define MASTER_OBJECT_H_

#include "structure.h"


/**newEnvironment: create new Environment structure
 *
 */

Environment newEnvironment(Object* objectList);

/* newObject: allocate memory for a object with initial data.
 * uint8_t x: x-coordinate of Object.
 * uint8_t y: y-coordinate of Object.
 * uint8_t *data: pointer to data array in memory.
 * return Object: returns pointer to newly allocated Object.
 */
Object newObject(uint8_t x, uint8_t y, const uint8_t *sprite);

/* free memory of Object.
 */
void releaseObject(Object instance);

/* Setter methods for object
 * setObjectXY: Change position values of object. This is does not equal moving the object.
 */
void setObjectXY(Object self, uint8_t x, uint8_t y);
void setObjectData(Object self, const uint8_t *data);

/* moveObject: Move the Object to the given location. Accesses blockList of main environment.
 * Object self: Object to be moved.
 * Environment mainEnv: main environment. blockList is manipulated to force redraw of previously occupied pages.
 * uint8_t x: x-coordinate to move to.
 * uint8_t y: y-coordinate to move to.
 */
void moveObject(Object self, Environment mainEnv, uint8_t x, uint8_t y);


/* mapObject: Create new Block from Object instance
 * Object instance: Object to be mapped to a Block of pages.
 * return Block: Resulting Block.
 */
Block mapObject(Object instance);



#endif /* MASTER_OBJECT_H_ */
