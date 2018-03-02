/*
 * block.c
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#include <stdlib.h>
#include "display.h"
#include "defines.h"
#include "block.h"
#include "char.h"
#include "buttons.h"

Block newBlock(uint8_t x, uint8_t y, uint8_t *data){
	Block self = (Block)malloc(sizeof(struct Block_struct));
	self->x = x;
	self->y = y;
	self->lx = data[0];
	self->ly = data[1]/data[0];
	self->data = data;
	self->blockType = NOTDRAWN;
	return self;
}

Block emptyBlock(uint8_t x, uint8_t y, uint8_t lx, uint8_t ly){
	Block self =  (Block)malloc(sizeof(struct Block_struct));
	self->x = x;
	self->y = y;
	self->lx = lx;
	self->ly = ly;
	self->data = 0;
	self->blockType = NOTDRAWN;
	return self;
}

void releaseBlock(Block instance){
	free(instance->data);
	free(instance);

}

void drawBlock(Block instance){
	if(!instance->data){
		sendWindow(instance->x, instance->y, instance->lx, instance->ly,0);
	}
	else{// if(instance->data[1]>3){
		sendWindow(instance->x, instance->y, instance->lx, instance->ly, instance->data);
	}
	/**
	else{
		uint8_t i;
		for(i=0; i< instance->data[1]; i++){
			page(instance->x, instance->y, instance->data[i+2]);
		}
	}
	**/

}

void checkBlockCollision(Object* objectList, uint8_t length){
	uint8_t i,j;
	Block a,b;


	for(i=0; i<length; i++){
		a = objectList[i]->representation;
		for(j=i+1; j<length; j++){
			b = objectList[j]->representation;
			if((a->blockType == NOTDRAWN || b->blockType == NOTDRAWN) && isColliding(a->x,a->y,a->lx,a->ly,b->x,b->y,b->lx,b->ly)){
				Block overlap = getOverlap(a,b);
				drawBlock(overlap);
				releaseBlock(overlap);
			}
		}
	}
}

Block getOverlap(Block a, Block b){
	uint8_t x=0,y=0,dx, dy, dlx, dly, posx, posy, poslx, posly;

	dx 		= a->x>b->x		?	a->x-b->x	:	b->x-a->x;
	dy 		= a->y>b->y		?	a->y-b->y	:	b->y-a->y;

	poslx = (a->x+a->lx)<(b->x+b->lx)?(a->x+a->lx):(b->x+b->lx);
	posly = (a->y+a->ly)<(b->y+b->ly)?(a->y+a->ly):(b->y+b->ly);

	posx 	= a->x>b->x		?	a->x		:	b->x;
	posy 	= a->y>b->y		?	a->y		:	b->y;

	dlx 	= poslx -posx;
	dly 	= posly -posy;

	uint8_t *ndata = calloc(dlx*dly +2 ,sizeof(uint8_t));
	ndata[0] = dlx;
	ndata[1] = dlx*dly;

	for(y=0;y<dly; y++){
		for(x=0; x<dlx; x++){

			if		(a->x <= b->x && a->y <= b->y){
				ndata[2 + x*dly + y] =  a->data[2 + (x+dx)	*a->ly + y + dy] |
										b->data[2 + (x)		*b->ly + y];


			}
			else if	(b->x < a->x && a->y <= b->y){
				ndata[2 + x*dly + y] =  a->data[2 + (x)		*a->ly + y + dy] |
										b->data[2 + (x+dx)	*b->ly + y];

			}
			else if	(a->x <= b->x && b->y < a->y){
				ndata[2 + x*dly + y] =  a->data[2 + (x+dx)	*a->ly + y ] |
										b->data[2 + (x)		*b->ly + y + dy];

			}
			else{
				ndata[2 + x*dly + y] =  a->data[2 + (x)		*a->ly + y ] |
										b->data[2 + (x+dx)	*b->ly + y + dy];

			}

		}
	}
	return newBlock(posx,posy,ndata);

}

void removeSpace(Block oldBlock, uint8_t x, uint8_t y){
	uint8_t dx, dy;
	dx = oldBlock->x > x?oldBlock->x-x :x-oldBlock->x;
	dy = oldBlock->y > y?oldBlock->y-y :y-oldBlock->y;
	//If there is no overlap between the new and old position of the Block
	//then draw white space in the shape of the old Block.
	if( dx> oldBlock->lx || dy > oldBlock->ly || (dx == 0 && dy==0)){
		sendWindow(oldBlock->x, oldBlock->y, oldBlock->lx, oldBlock->ly, 0);
		return;
	}

	//Block0:
	uint8_t nx,ny,lx,ly;
	if(dy!=0){

		if( oldBlock->y < y){
			nx = oldBlock->x;
			ny = oldBlock->y;
		}
		else{
			nx = oldBlock->x;
			ny = oldBlock->y + oldBlock->ly - dy;
		}

		ly = dy;

		if(oldBlock->y > y && y%4==0){
			ny = ny ==0 ? 0 : ny-1;
			ly = ny + ly +1 < MAXY ? ly +1: ly;
		}

		lx = oldBlock->lx;
		sendWindow(nx,ny,lx,ly,0);
	}

	//Block1:
	if(dx !=0){
		if(oldBlock->y < y){
			if(oldBlock->x < x){
				nx = oldBlock->x;
				ny = oldBlock->y + abs(dy);
			}
			else{
				nx = oldBlock->x +oldBlock->lx - dx;
				ny = oldBlock->y + abs(dy);
			}
		}
		else{
			if(oldBlock->x < x){
				nx = oldBlock->x;
				ny = oldBlock->y;
			}
			else{
				nx = oldBlock->x +oldBlock->lx - dx;
				ny = oldBlock->y;

			}
		}
		lx = dx;
		ly = oldBlock->ly - dy;
		sendWindow(nx,ny,lx,ly,0);
	}

}


uint8_t isColliding(uint8_t x0,uint8_t y0,uint8_t lx0,uint8_t ly0,uint8_t x1,uint8_t y1,uint8_t lx1,uint8_t ly1){
	int8_t dx, dy;
	uint8_t checkVal=0;
	dx = x1-x0;
	dy = y1-y0;


	if(dx >= 0){
		if(dx <lx0){
			checkVal++;

		}
	}
	else if(dx < 0){
		if(abs(dx) <lx1){
			checkVal++;
		}
	}

	if(dy >= 0){
		if(dy <ly0){
			checkVal++;

		}
	}
	else if(dy <0){
		if(abs(dy) <ly1){
			checkVal++;

		}
	}
	if(checkVal==2){
		print("YES",0,2);
		return 1;
	}
	else{
		print("NO ",0,2);
		return 0;
	}
}

