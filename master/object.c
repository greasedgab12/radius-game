#include "object.h"
#include "block.h"
#include "environment.h"

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
		//Set first two entries to width of sprite and overall length of the array.
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
		block->y = instance->y/4;
		block->ly = instance->ly;
		block->data = instance->data;
		return block;
	}
}
