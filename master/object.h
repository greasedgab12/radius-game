#ifndef MASTER_OBJECT_H_
#define MASTER_OBJECT_H_

#include "structure.h"



/* newObject: allocate memory for a object with initial data.
 * uint8_t x: x-coordinate of Object.
 * uint8_t y: y-coordinate of Object.
 * uint8_t *data: pointer to data array in memory.
 * return Object: returns pointer to newly allocated Object.
 */
Object newObject(uint8_t x, uint8_t y, uint8_t lx, uint8_t ly, const uint8_t* sprite);

/** releaseObject: free memory of given object instance.
 * Object instance: Object to be freed.
 */
void releaseObject(Object instance);


/* Setter methods for object
 * setObjectXY: Change position values of object. This is does not equal moving the object.
 */
void setObjectXY(Object self, uint8_t x, uint8_t y);
void setObjectData(Object self, const uint8_t* sprite);

/**drawObject: map and draw Object on screen.
**/
void drawObject(Object instance);

/**removeSpace: draw white pixels on screen in places previously occupied by the moving object.
 * Object instance: object instance.
 * uint8_t x: absolute x coordinate object instance moves to.
 * uint8_t y: absolute y coordinate object instance moves to.
 */
void removeSpace(Object instance, uint8_t x, uint8_t y);


void checkMappedSpriteCollision(Object* objectList, uint8_t length);

void drawOverlap(Object a, Object b);

uint8_t isColliding(Object self, Object other, int8_t rx, int8_t ry);

uint8_t isMappedColliding(Object self, Object other, int8_t rx, int8_t rpy);
/* moveObject: Move the Object by relative coordniates . Accesses blockList of main environment.
 * Object self: Object to be moved.
 * Environment mainEnv: main environment. blockList is manipulated to force redraw of previously occupied pages.
 * int8_t x: x-coordinate to move to.
 * int8_t y: y-coordinate to move to.
 */
void moveObject(Object self, Environment mainEnv, int8_t r_x, int8_t r_y);



uint8_t* mapData(const uint8_t* data, uint8_t ty);


#endif /* MASTER_OBJECT_H_ */
