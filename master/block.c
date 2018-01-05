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
	//k takes the value of usedIndices[i] while pos is next free entry of the overlaps array.
	uint8_t i,j,k,pos=0;
	//Since we don't want Blocks to collide with multiple other Blocks, indices of Blocks
	//that were involved in a collision are set to zero. The indices are saved in the usedIndices array.
	uint8_t* usedIndices = malloc(length*sizeof(uint8_t));
	//Array of our detected overlaps. Must be terminated with zero and is therefore one entry larger than MAXOVERLAPS.
	Block* overlaps = (Block*)malloc((1 + MAXOVERLAPS )* sizeof(Block));
	for(i=0;i<length;i++){
		usedIndices[i] = i;
	}

	for(i=0; i<length; i++){
		for(j=i+1; j<length; j++){
			//Only query the Blocks if the index is non zero!
			if(usedIndices[j]){
				k=usedIndices[j];
				//Find out, if Block[k] has coordinates within Block[i] borders.
				if( (blockList[i]->x <= blockList[k]->x) &&
					(blockList[k]->x <= blockList[i]->x + blockList[i]->lx) &&
					(blockList[i]->y <= blockList[k]->y)&&
					(blockList[k]->y <= blockList[i]->y + blockList[i]->ly)){
					//When true, mark index of Block[k] as used.
					usedIndices[j]=0;
					//Write new Block into overlaps, unless maxium capacity is reached.
					if(pos < MAXOVERLAPS){
						overlaps[pos] = getOverlap(blockList[i], blockList[k]);
						pos++;
					}
					else{
						//If we are at maximum capacitance. Return overlap and ignore other potential collisions.
						free(usedIndices);
						overlaps[pos]=0;
						return overlaps;
					}
				}
			}
		}
	}
	free(usedIndices);
	overlaps[pos] =0;
	return overlaps;
}

Block getOverlap(Block b1, Block b2){
	uint8_t x=0,y=0,dx, dy, dlx, dly;

	dx = b1->x>b2->x ? b1->x-b2->x: b2->x-b1->x;
	dy = b1->y>b2->y ? b1->y-b2->y: b2->y-b1->y;

	dlx = b1->x>b2->x ? b2->lx-dx: b1->lx-dx;
	dly = b1->y>b2->y ? b2->ly-dy: b1->ly-dy;

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
