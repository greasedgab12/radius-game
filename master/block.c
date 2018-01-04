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

Block* checkBlockCollision(Block* blockList, uint8_t length){

;}

Block getOverlap(Block b1, Block b2){
	uint8_t x,y,dx, dy, dlx, dly;
	dx = b1->x>b2->x ? b1->x-b2->x: b2->x-b1->x;
	dy = b1->y>b2->y ? b1->y-b2->y: b2->y-b1->y;
	dlx = b1->x>b2->x ? b1->lx-b2->lx: b2->lx-b1->lx;
	dly = b1->y>b2->y ? b1->ly-b2->ly: b2->ly-b1->ly;

	uint8_t *ndata = calloc(dlx*dly +2 ,sizeof(uint8_t));
	ndata[0] = dlx;
	ndata[1] = dlx*dly;

	for(x=0; x< dlx; x++){
		for(y=0; y< dly; y++){
			if(b2->x > b1->x && b2->y > b1->y){
				ndata[2 + x*dly + y] = b1->data[2 + (x+dx)*b1->ly + y +dy] | b2->data[2 + x*b2->ly + y];
			}
			else if(b1->x > b2->x && b2->y > b1->y){
				ndata[2 + x*dly + y] = b1->data[2 + x*b1->ly + y +dy] | b2->data[2 + (x+dx)*b2->ly + y];
			}
			else if(b2->x > b1->x && b1->y > b2->y){
				ndata[2 + x*dly + y] = b1->data[2 + (x+dx)*b1->ly + y ] | b2->data[2 + (x+dx)*b2->ly + y+dy];
			}
			else if(b1->x > b2->x && b1->y > b2->y){
				ndata[2 + x*dly + y] = b1->data[2 + x*b1->ly + y] | b2->data[2 + (x+dx)*b2->ly + y + dy];
			}
		}
	}
	Block nb = newBlock(x,y,ndata);
	return nb;

}
