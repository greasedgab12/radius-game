#include <stdlib.h>
#include "object.h"
#include "display.h"
#include "defines.h"
#include "block.h"
#include "timer.h"

Environment newEnvironment(Object* objectList){
	Environment self = (Environment)malloc(sizeof(struct Environment_struct));
    self->time = getMsTimer();
    self->gameState =0;
    self->bPos = 0;
    self->objectList = objectList;
    self->oPos = 0;
    self->buttons =0;
    return self;
}

Object newObject(uint8_t x, uint8_t y, const uint8_t *data){

    Object self = (Object)malloc(sizeof(struct Object_Struct));

    self->x = x;
    self->y = y;

    self->setData = &setObjectData;
    self->setData(self, data);
    self->setXY = &setObjectXY;
    self->move = &moveObject;
    self->representation =0;
    return self;
}

void releaseObject(Object instance){
	releaseBlock(instance->representation);
	free(instance->objectEnv);
	free(instance);
}

void setObjectXY(Object self, uint8_t x, uint8_t y){
    self->x = x;
    self->y = y;
}

void setObjectData(Object self, const uint8_t *data){
    self->lx = data[0];
    self->ly = data[1]/data[0];
    self->data= data;
}

Block mapObject(Object instance){
	uint8_t offset,x,y,bx,by,blx,bly;
	uint8_t *ndata;
	offset = (instance->y%4)*2;
	bx = instance->x;
	blx = instance->lx;
	if(offset){
		by = instance->y/4;
		bly = instance->ly+1;
		//Allocate memory for ndata array.
		ndata = calloc((blx*bly + 2),sizeof(uint8_t));
		//Set first two entries to width of Object and overall length of the array.
		ndata[0] = blx;
		ndata[1] = ndata[0]*(bly);
		for(x=0; x<blx; x++){
			//Set the first row to the orignal data shifted to the left by offset.
			ndata[2 + x*(bly)] = instance->data[2+ x*instance->ly]<<(offset);
			for(y=1; y<instance->ly; y++){
				//Set all but last row to the union of the current and previos row of the original data, shifted left and right by offset.
				ndata[2 + x*(bly) +y] = (instance->data[2+ x*instance->ly +y-1]>>(8-offset))|(instance->data[2+ x*instance->ly +y]<<(offset));
			}
			//Set the last row to the orignal data shifted to the left by offset.
			ndata[2 + (x+1)*bly -1] = (instance->data[2+ x*instance->ly +y -1]>>(8-offset));
		}
		return newBlock(bx,by,ndata);
	}
	else{
		//Without offset only the y coordinate is mapped.
		ndata = calloc(instance->data[1] +2, sizeof(uint8_t));
		ndata[0] = blx;
		ndata[1] = instance->data[1];
		for(x=2; x<instance->data[1]+2; x++){
			ndata[x] = instance->data[x];
		}
		by = instance->y/4;
		bly = instance->ly;
		return newBlock(bx,by,ndata);
	}
}
/**ToDO: Resolve drawing error with big objects near the upper border.
 *
 */
void moveObject(Object self, Environment mainEnv, uint8_t x, uint8_t y){
	uint8_t i;
	Block repr = self->representation;
	/**
	//If the coordinates stay the same, then nothing is to be done.
	if((x == self->x && y == self->y)){
			return;
	}
	 **/

	//Collision detection.

	//Check for collisions with borders:
	//Set the target coordinates to the last valid ones before the collision.

	if(x<=MINX || x +self->lx >= MAXX){
		int8_t ov_x;
		ov_x = x<=MINX? MINX -x +1: MAXX - (x + self->lx + 1);
		x+= ov_x;
	}
	if( y<=MINY || y + self->ly*4 >= MAXY){
		int8_t ov_y;
		ov_y = y<=MINY? MINY -y +1: MAXY - (y + self->ly*4  +1);
		y+= ov_y;
	}


	for(i=0; i< mainEnv->oPos; i++){
		if(mainEnv->objectList[i]!= self){
			Object other = mainEnv->objectList[i];
			if(isColliding(x, y, self->lx, self->ly*4 ,
					other->x,other->y, other->lx, other->ly*4)){
				if(self->collide(self, other)){
					uint8_t tx,ty;
					if(self->x < other->x){
						tx = other->x - self->lx;
					}
					else{
						tx = other->x +other->lx;
					}
					if(self->y < other->y){
						ty = other->y -self->ly*4;
					}
					else{
						ty = other->y +other->ly*4;
					}
					uint8_t dx,dy;

					dx = self->x > tx? self->x - tx : tx - self->x;
					dy = self->y > ty? self->y - ty : ty - self->y;

					if(tx*tx+y*y <= x*x+ty*ty){
						x=tx;
					}
					else{
						y=ty;

					}
					printN(dx,50,0);
					printN(dy,50,2);
					break;
				}

			}

}
	}
	/**
	//If the coordinates stay the same, then nothing is to be done.
		if((x == self->x && y == self->y)){
				return;
		}
	**/
	//Actual Movement of the Object.
	//Search for collisions with blocks and set their state to NOTDRAWN to force redraw.
	for(i=0; i<mainEnv->oPos; i++){
		if(mainEnv->objectList[i]->representation != repr){
			if( isColliding(repr->x, repr->y, repr->lx, repr->ly,
					mainEnv->objectList[i]->representation->x, mainEnv->objectList[i]->representation->y,
					mainEnv->objectList[i]->representation->lx, mainEnv->objectList[i]->representation->ly)){

							mainEnv->objectList[i]->representation->blockType = NOTDRAWN;
					}
		}
	}

	removeSpace(repr, x, y);

	if(self->y%4 != y%4){
		self->x = x;
		self->y = y;
		releaseBlock(self->representation);
		self->representation = mapObject(self);

	}
	else{
		self->x = x;
		self->y = y;
		repr->x = x;
		repr->y = y/4;
		repr->blockType = NOTDRAWN;
	}



}

