#include "object.h"
#include "block.h"
#include <stdlib.h>

Object newObject(uint8_t x, uint8_t y, const uint8_t *data){

    Object self = (Object)malloc(sizeof(struct Object_Struct));

    self->x = x;
    self->y = y;

    self->setData = &setObjectData;
    self->setData(self, data);
    self->setXY = &setObjectXY;
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
	uint8_t offset,x,y;
	uint8_t *ndata;
	Block block = (Block)malloc(sizeof(struct Block_struct));
	offset = (instance->y%4)*2;
	block->x = instance->x;
	block->lx = instance->lx;
	if(offset){
		block->y = instance->y/4 - offset/2;
		block->ly = instance->ly+1;
		//Allocate memory for ndata array.
		ndata = calloc((block->lx*(block->ly) + 2),sizeof(uint8_t));
		//Set first two entries to width of Object and overall length of the array.
		ndata[0] = block->lx;
		ndata[1] = ndata[0]*(block->ly);
		for(x=0; x<block->lx; x++){
			//Set the first row to the orignal data shifted to the left by offset.
			ndata[2 + x*(block->ly)] = instance->data[2+ x*instance->ly]<<(8-offset);
			for(y=1; y<instance->ly; y++){
				//Set all but last row to the union of the current and previos row of the original data, shifted left and right by offset.
				ndata[2 + x*(block->ly) +y] = (instance->data[2+ x*instance->ly +y-1]>>offset)|(instance->data[2+ x*instance->ly +y]<<(8-offset));
			}
			//Set the last row to the orignal data shifted to the left by offset.
			ndata[2 + (x+1)*block->ly -1] = (instance->data[2+ x*instance->ly +y -1]>>offset);
		}

		block->data = ndata;
		return block;
	}
	else{
		//Without offset only the y coordinate is mapped.
		ndata = calloc(instance->data[1] +2, sizeof(uint8_t));
		ndata[0] = block->lx;
		ndata[1] = instance->data[1];
		block->y = instance->y/4;
		block->ly = instance->ly;
		block->data = ndata;
		return block;
	}
}

void moveObject(Object self, Environment mainEnv, uint8_t x, uint8_t y){
	uint8_t i,dx,dy;

	for(i=0; i< mainEnv->oPos; i++){
		if( (x <= mainEnv->objectList[i]->x) &&
			(mainEnv->objectList[i]->x <= x + self->lx) &&
			(y <= mainEnv->objectList[i]->y)&&
			(mainEnv->objectList[i]->y <= y + self->ly)){
			if(self->collide(self, mainEnv->objectList[i])){
				return;
			}
		}
	}
	Block repr = self->representation;
	for(i=0; i<mainEnv->bPos; i++){
		if( (repr->x <= mainEnv->blockList[i]->x) &&
			(mainEnv->objectList[i]->x <= repr->x + repr->lx) &&
			(repr->y <= mainEnv->blockList[i]->y)&&
			(mainEnv->blockList[i]->y <= repr->y + repr->ly)){
			blockList[i]->blockType = NOTDRAWN;
		}
	}
	self->x = x;
	if(self->y%4 == y%4){
		repr->x = x;
		repr->y = y;
	}
	else{
		self->y = y;
		self->representation = mapObject(self);
		return;
	}
	self->y = y;
}
