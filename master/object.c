#include <stdlib.h>
#include "object.h"
#include "display.h"
#include "defines.h"

Environment newEnvironment(Object* objectList, Block* blockList){
	Environment self = (Environment)malloc(sizeof(struct Environment_struct));
    self->time2 = getMsTimer();
    self->gameState =0;
    self->blockList = blockList;
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

void moveObject(Object self, Environment mainEnv, uint8_t x, uint8_t y){
	uint8_t i;
	Block repr = self->representation;
	if((x == self->x && y == self->y) ||x<=0 || x +self->lx >= MAXX || y<=0 || y + self->ly*4 >= MAXY){
		return;
	}

	removeSpace(repr, x, y);

	for(i=0; i< mainEnv->oPos; i++){
		if( isColliding(x,y,self->lx, self->ly,
				mainEnv->objectList[i]->x, mainEnv->objectList[i]->y,
				mainEnv->objectList[i]->lx, mainEnv->objectList[i]->ly)){
			if(self->collide(self, mainEnv->objectList[i])){
				return;
			}
		}
	}
	//Search for collisions with blocks and set their state to NOTDRAWN to force redraw.
	for(i=0; i<mainEnv->oPos; i++){
		if( isColliding(repr->x, repr->y, repr->lx, repr->ly,
				mainEnv->objectList[i]->representation->x, mainEnv->objectList[i]->representation->y,
				mainEnv->objectList[i]->representation->lx, mainEnv->objectList[i]->representation->ly)){
					if(mainEnv->objectList[i]->representation != repr){
						mainEnv->objectList[i]->representation->blockType = NOTDRAWN;


					}
		}
	}

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

