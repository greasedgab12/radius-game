#include "sprite.h"
#include <stdlib.h>


Sprite newSprite(uint8_t x, uint8_t y, const uint8_t *data){

    Sprite self = (Sprite)malloc(sizeof(struct Sprite_Struct));

    self->x = x;
    self->y = y;

    self->setData = &setSpriteData;
    self->setData(self, data);
    self->setXY = &setSpriteXY;
    return self;
}

void setSpriteXY(Sprite self, uint8_t x, uint8_t y){
    self->x = x;
    self->y = y;
}

void setSpriteData(Sprite self, const uint8_t *data){
    self->lx = data[0];
    self->ly = data[1]/data[0];
    self->data= data;
}

Blank newBlank(Sprite instance){
    Blank self = (Blank)malloc(sizeof(struct Blank_Struct));

    self->x = instance->x;
    self->y = instance->y;
    self->lx = instance->lx;
    self->ly = instance->ly;
    return self;
}

Sprite mapSprite(Sprite instance){
	uint8_t offset,x,y;
	uint8_t *ndata;
	offset = (instance->y%4)*2;
	if(offset){
		//Allocate memory for ndata array.
		ndata = calloc((instance->lx*(instance->ly+1) + 2)*sizeof(uint8_t));
		//Set first two entries to width of sprite and overall length of the array.
		ndata[0] = instance->lx;
		ndata[1] = ndata[0]*(instance->ly+1);
		for(x=0; x<instance->lx; x++){
			//Set the first row to the orignal data shifted to the right by offset.
			ndata[2 + x*instance->ly] = instance->data[2+ x*instance->ly]<<offset;
			for(y=1; y<instance->ly; y++){
				//Set all but last row to the union of the current and previos row of the original data, shifted left and right by offset.
				ndata[2 + x*instance->ly +y] = (instance->data[2+ x*instance->ly +y-1]>>offset)|(instance->data[2+ x*instance->ly +y]<<offset);
			}
			//Set the last row to the orignal data shifted to the left by offset.
			ndata[2 + (x+1)*instance->ly] = (instance->data[2+ x*instance->ly +y]<<offset);
		}
	}
	else{
		//Without any offset, the isntance ist returned.
		return instance;
	}
	//return new Sprite from values of instance and ndata.
	return newSprite(instance->x, instance->y - offset/2, ndata);
}
