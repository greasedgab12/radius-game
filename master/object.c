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
	if(x<0 || x +self->lx > MAXX || y<0 || y + self->ly > MAXY){
		return;
	}

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


	for(i=0; i<mainEnv->bPos; i++){
		if( (repr->x <= mainEnv->blockList[i]->x) &&
			(mainEnv->objectList[i]->x <= repr->x + repr->lx) &&
			(repr->y <= mainEnv->blockList[i]->y)&&
			(mainEnv->blockList[i]->y <= repr->y + repr->ly)){
			if(mainEnv->blockList[i] != repr){
				mainEnv->blockList[i]->blockType = NOTDRAWN;
			}
		}
	}

	if(self->y%4 != y%4){
		self->x = x;
		self->y = y;
		self->representation = mapObject(self);

	}
	else{
		self->x = x;
		self->y = y;
		repr->x = x;
		repr->y = y/4;
	}


}
