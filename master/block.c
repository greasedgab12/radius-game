/*
 * block.c
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#include <stdlib.h>
#include "block.h"
#include "display.h"

Block newBlock(uint8_t x, uint8_t y, uint8_t *data){
	Block block = (Block)malloc(sizeof(struct Block_struct));
	block->x = x;
	block->y = y;
	block->lx = data[0];
	block->ly = data[1]/data[0];
	block->data = data;
	return block;
}

void releaseBlock(Block instance){
	uint8_t length = instance->data[1]+2;
	uint8_t i;
	for(i=0; i<length; i++){
		free(&instance->data[i]);
	}
	free(instance->data);
	free(instance);

}

void drawBlock(Block instance){
	if(instance->data[1]>3){
		sendWindow(instance->x, instance->y, instance->lx, instance->ly, instance->data);
	}
	else{
		uint8_t i;
		for(i=0; i< instance->data[1]; i++){
			page(instance->x, instance->y, instance->data[i+2]);
		}
	}
}
