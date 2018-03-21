#ifndef MASTER_OBJECT_H_
#define MASTER_OBJECT_H_

#include "structure.h"


/**initObject: Initialize Object attributes as well as general function pointers.
 * Object self: Object instance.
 */
void initObject(Object self);

/* newObject: Set Object attributes with given parameters.
 * Object self: Object instannce.
 * uint8_t x: x-coordinate of Object.
 * uint8_t y: y-coordinate of Object.
 * uint8_t *data: pointer to data array in memory.
 */
void newObject(Object self,uint8_t x, uint8_t y, uint8_t lx, uint8_t ly, const uint8_t* sprite);


/* Setter methods for object
 * setObjectXY: Change position values of object. This is does not equal moving the object.
 * setObjectData: Change object sprite.
 */
void setObjectXY(Object self, uint8_t x, uint8_t y);
void setObjectData(Object self, const uint8_t* sprite);

/**drawObject: Map and draw Object on screen.
 * Object instance: Object instance.
**/
void drawObject(Object instance);

/**removeSpace: draw white pixels on screen in places previously occupied by the moving object.
 * Object instance: object instance.
 * uint8_t x: absolute x coordinate object instance moves to.
 * uint8_t y: absolute y coordinate object instance moves to.
 */
void removeSpace(Object instance, uint8_t x, uint8_t y);

/** checkMappedSpriteCollision: Check wether objects in objectList have colliding mapped sprites.
 * 								Calls drawOverlap on colliding objects.
 *	Object* objectList: List of objects.
 */
void checkMappedSpriteCollision(Object* objectList);

/**drawOverlap: Fuse data of overlapping area between two objects and redraw the result.
 * Object a: First object instance.
 * Object b: Second object instance.
 */
void drawOverlap(Object a, Object b);

/**isColliding: Checks wether the first object collides physically with the second if moved by relative coordinates.
 * Object self: First object instance.
 * Object other: Second object instance.
 * uint8_t rx,ry: Relative Coordinates.
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t isColliding(Object self, Object other, int8_t rx, int8_t ry);


/**isColliding: Checks wether the first object collides sprite wise with the second if moved by relative coordinates.
 * Object self: First object instance.
 * Object other: Second object instance.
 * uint8_t rx,rpy: Relative coordinates with rpy being the page coordinate. .
 * return uint8_t: 0: no collision, 1: collision.
 */
uint8_t isMappedColliding(Object self, Object other, int8_t rx, int8_t rpy);

/* moveObject: Move the Object by relative coordinates . Accesses blockList of main environment.
 * Object self: Object to be moved.
 * Environment env: main environment. blockList is manipulated to force redraw of previously occupied pages.
 * int8_t rx: x-coordinate to move to.
 * int8_t ry: y-coordinate to move to.
 */
void moveObject(Object self, Environment env, int8_t rx, int8_t ry);


/** mapData: mapData to offset given by pixel position ty.
 * const uint8_t* data: Pointer to the sprite in memory.
 * uint8_t ty: target pixel position.
 * return uint8_t*: array of the mapped sprite.
 */
uint8_t* mapData(const uint8_t* data, uint8_t ty);


#endif /* MASTER_OBJECT_H_ */
