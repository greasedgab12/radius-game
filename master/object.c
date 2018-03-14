#include <stdlib.h>
#include "object.h"
#include "display.h"
#include "defines.h"
#include "timer.h"
#include "sprite.h"
#include "entity.h"



void initObject(Object self){
	self->x = 0;
    self->y = 0;
    self->killedBy = 0;
    self->lx =0;
    self->ly =0;
    self->msly = 0;
    self->drawState = NOTDRAWN;
    self->activeState = EMPTY;
    self->setXY = &setObjectXY;
    self->setData = &setObjectData;

}

void newObject(Object self, uint8_t x, uint8_t y, uint8_t lx, uint8_t ly,const  uint8_t* sprite){

    self->x = x;
    self->y = y;
    self->killedBy = 0;
    self->lx =lx;
    self->ly =ly;
    self->setData(self, sprite);
    self->drawState = NOTDRAWN;

}



void setObjectXY(Object self, uint8_t x, uint8_t y){
    if(x + self->lx > (MAXX)){
    	x=MAXX - self->lx;
    }

	self->x = x;

	if(y + self->ly  >= (MAXY )){
		y=(MAXY - self->ly);
	}
    self->y = y;

    self->msly = self->sly;
    self->msly += y%4?1:0;
}

void setObjectData(Object self, const uint8_t* sprite){
    self->data = load_sprite(sprite);
	self->slx = self->data[0];
	self->sly = self->data[1] / self->data[0];
    self->msly = self->sly;
    self->msly+=(self->y%4?1:0);
}

void drawObject(Object instance){
	uint8_t *mdata;
	uint8_t i;
	mdata = mapData(instance->data, instance->y);
	for(i=0; i<instance->lx/mdata[0]; i++){
		if(instance->x +i + mdata[0] < MAXX){
			sendWindow(instance->x + i, instance->y/4,mdata[0], mdata[1]/mdata[0], mdata);
		}
	}
	free(mdata);
}
uint8_t* mapData(const uint8_t* data, uint8_t ty){
	uint8_t offset,ply,lx, ly, x, y;
	uint8_t* ndata;
	offset = (ty%4)*2;
	lx = data[0];
	ly = data[1]/lx;
	if(offset){
		ply = data[1]/data[0] +1;
		//Allocate memory for ndata array.
		ndata = (uint8_t*)malloc((lx*ply + 2)*sizeof(uint8_t));


		//Set first two entries to width of Object and overall length of the array.
		ndata[0] = lx;
		ndata[1] = lx*(ply);
		for(x=0; x<lx; x++){
			//Set the first row to the orignal data shifted to the left by offset.
			ndata[2 + x*(ply)] = data[2+ x*ly]<<(offset);
			for(y=1; y<ly; y++){
				//Set all but last row to the union of the current and previos row of the original data, shifted left and right by offset.
				ndata[2 + x*(ply) +y] = (data[2+ x*ly +y-1]>>(8-offset))|(data[2+ x*ly +y]<<(offset));
			}
			//Set the last row to the orignal data shifted to the left by offset.
			ndata[2 + (x+1)*ply -1] = (data[2+ x*ly +y -1]>>(8-offset));
		}

	}
	else{
		//Without offset only the y coordinate is mapped.
		ndata = (uint8_t*)malloc(data[1] +2 *sizeof(uint8_t));
		ndata[0] = lx;
		ndata[1] = data[1];
		for(x=2; x<data[1]+2; x++){
			ndata[x] = data[x];
		}
	}
	return ndata;

}
void moveObject(Object self, Environment mainEnv, int8_t rx, int8_t ry){
	uint8_t i;


	//If the coordinates stay the same, then nothing is to be done.
	if((rx == 0 && ry == 0)){
			return;
	}

	//Collision detection.

	//Check for collisions with borders:
	//Set the target coordinates to the last valid ones before the collision.
	uint8_t x = self->x +rx;
	uint8_t y = self->y +ry;
	if(rx < 0 && rx < MINX-self->x){
		x = MINX;
		self->collide(self, 0, LEFTC, 0);
	}
	else if(rx >0 && rx > MAXX-self->x-self->lx){
		x = MAXX-self->lx;
		self->collide(self, 0, RIGHTC, 0);
	}
	if(ry < 0 && ry < MINY-self->y-1){
		y = MINY;
		self->collide(self, 0, UPPERC, 0);
	}
	else if(ry >0 && ry > MAXY-self->y-self->ly){
		y = MAXY-self->ly-1;
		self->collide(self, 0, LOWERC, 0);
	}

	//Check if the Object collides with other objects in the main Environment.
	for(i=0; i< MAXOBJECTS; i++){
		//Objects cannot collide with themselves.
		if(mainEnv->objectList[i]!= self && (mainEnv->objectList[i]->activeState == ACTIVE)){
			//ToDoRemove variable other in final build.
			Object other = mainEnv->objectList[i];
			uint8_t collisionType;
			collisionType = isColliding(self, other,rx, ry);
			//Is self colliding with an object at the new position?
			if(collisionType){
				//Only change the movement target, if self and the collision partner are allowed to collide.
				if(self->collide(self, other,collisionType,0)){
					//Change movement target to the closest position before collision with the object.
					if(collisionType ==LEFTC){
						x = other->x - self->lx;
					}
					else if(collisionType ==RIGHTC){
						x = other->x +other->lx;
					}
					else if(collisionType ==UPPERC){
						y = other->y -self->ly;

					}
					else if(collisionType ==LOWERC){
						y = other->y +other->ly;
					}
					x = x<MAXX?x:MAXX-self->lx;
					x = x>MINX?x:MINX;
					y = y<MAXY?y:MAXY-self->ly;
					y = y>MINY?y:MINY;

					/**
					//Move only to the positon closest to the current position.
					if(tx*tx+y*y <= x*x+ty*ty){
						x=tx;
					}
					else{
						y=ty;

					}
					//An Object may only collide once during movement.
					**/
					break;
				}
			}

		}
	}

	//If the coordinates stay the same, then nothing is to be done.
		if((x == self->x && y == self->y)){
				return;
		}

	//Actual Movement of the Object.
	/**Search for collisions with blocks at the current position and set their state to NOTDRAWN to force redraw.
	 * This won't leave a 'hole' in overlapped blocks when the block moves.
	 */
	for(i=0; i<MAXOBJECTS; i++){
		Object other = mainEnv->objectList[i];
		if(self != other){
			if(isMappedColliding(self, other ,0,0)){
				mainEnv->objectList[i]->drawState = NOTDRAWN;
			}
		}
	}
	//While other blocks underneath will be redrawn after movement, empty space must be redrawn seperately.
	removeSpace(self,x, y);
	//Should the y coordinate be of a different modulus of the target y coordinate, then the object has to be mapped to that modulus.


	self->setXY(self,x,y);
	self->drawState = NOTDRAWN;



}

void removeSpace(Object instance, uint8_t x, uint8_t y){

	uint8_t dx, dy;
	dx = instance->x > x?instance->x-x :x-instance->x;
	dy = instance->y > y?instance->y-y :y-instance->y;
	//If there is no overlap between the new and old position of the Block
	//then draw white space in the shape of the old Block.
	if( dx> instance->slx || dy > instance->msly || (dx == 0 && dy==0)){


		sendWindow(instance->x, instance->y/4, instance->slx, instance->msly, 0);
		return;
	}

	//Block0:
	uint8_t nx,ny,slx,msly;
	if(dy!=0){

		if( instance->y/4 < y/4){
					nx = instance->x;
			ny = instance->y/4;
		}
		else{
			nx = instance->x;
			ny = instance->y/4 + instance->msly - dy;
		}

		msly = dy;

		if(instance->y/4 > y && y%4==0){
			ny = ny ==0 ? 0 : ny -1;
			msly = ny + msly +1 < MAXY ? msly +1: msly;
		}

		slx = instance->slx;
		sendWindow(nx,ny,slx,msly,0);
	}

	//Block1:
	if(dx !=0){
		if(instance->y/4 < y/4){
			if(instance->x < x){
				nx = instance->x;
				ny = instance->y/4 + abs(dy);
			}
			else{
				nx = instance->x +instance->slx - dx ;
				ny = instance->y/4 + abs(dy);
			}
		}
		else{
			if(instance->x < x){
				nx = instance->x;
				ny = instance->y/4;
			}
			else{
				nx = instance->x +instance->slx - dx ;
				ny = instance->y/4;

			}
		}

		slx = dx;
		msly = instance->msly - dy;
		sendWindow(nx,ny,slx,msly,0);
	}

}
uint8_t isColliding(Object self, Object other, int8_t rx, int8_t ry){
	int8_t dx, dy;
	uint8_t checkVal=0;
	if(self->activeState != ACTIVE || other->activeState !=ACTIVE){
		return NOC;
	}
	dx = other->x-self->x-rx;
	dy = other->y-self->y-ry;
	if(dx > 0){
		if(dx <self->lx){
			checkVal++;

		}
	}
	else if(dx <= 0){
		if(abs(dx) <other->lx){
			checkVal++;
		}
	}

	if(dy > 0){
		if(dy <self->ly){
			checkVal++;

		}
	}
	else if(dy <=0){
		if(abs(dy) <other->ly){
			checkVal++;

		}
	}

	if(checkVal==2){
		//After we know a collision took place, query collision type.
		dx = other->x - self->x;
		dy = other->y - self->y;
		if(abs(rx)>abs(ry)){
			if(dx>0){
				return  LEFTC;
			}
			else{
				return RIGHTC;
			}
		}
		else{
			if(dy>0){
				return UPPERC;
			}
			else{
				return LOWERC;
			}
		}
	}
	else{
		return NOC;
	}
}

uint8_t isMappedColliding(Object self, Object other, int8_t rx, int8_t rpy){
	int8_t dx, dy;
	uint8_t checkVal=0;
	if(self->activeState != ACTIVE || other->activeState !=ACTIVE){
		return NOC;
	}
	dx = other->x-self->x-rx;
	dy = other->y/4-self->y/4-rpy;
	if(dx > 0){
		if(dx <self->slx){
			checkVal++;

		}
	}
	else if(dx <= 0){
		if(abs(dx) <other->slx){
			checkVal++;
		}
	}

	if(dy > 0){
		if(dy <self->msly){
			checkVal++;

		}
	}
	else if(dy <=0){
		if(abs(dy) <other->msly){
			checkVal++;

		}
	}

	if(checkVal==2){
		return 1;
	}
	else{
		return 0;
	}
}

void checkMappedSpriteCollision(Object* objectList){
	uint8_t i,j;
	Object a,b;


	for(i=0; i<MAXOBJECTS; i++){
		a = objectList[i];
		if(a->activeState == ACTIVE){
			for(j=i+1; j<MAXOBJECTS; j++){
				b = objectList[j];
				if(b->activeState == ACTIVE){
					if((a->drawState == NOTDRAWN || b->drawState == NOTDRAWN)
							&& isMappedColliding(a,b,0,0)){
						drawOverlap(a,b);
					}
				}
			}
		}
	}
}

void drawOverlap(Object a, Object b){
	uint8_t x=0,y=0,dx, dy, dlx, dly, posx, posy, poslx, posly;
	uint8_t *adata, *bdata;

	adata = mapData(a->data,a->y);
	bdata = mapData(b->data,b->y);

	dx 		= a->x>b->x		?	a->x-b->x	:	b->x-a->x;
	dy 		= a->y/4>b->y/4	?	a->y/4-b->y/4	:	b->y/4-a->y/4;

	poslx = (a->x+a->slx)<(b->x+b->slx)		?(a->x+a->slx)	:(b->x+b->slx);
	posly = (a->y/4+a->msly)<(b->y/4+b->msly)	?(a->y/4+a->msly):(b->y/4+b->msly);

	posx 	= a->x>b->x		?	a->x		:	b->x;
	posy 	= a->y/4>b->y/4	?	a->y/4		:	b->y/4;

	dlx 	= poslx -posx;
	dly 	= posly -posy;

	uint8_t *ndata = (uint8_t*)malloc(dlx*dly +2 * sizeof(uint8_t));
	ndata[0] = dlx;
	ndata[1] = dlx*dly;

	for(y=0;y<dly; y++){
		for(x=0; x<dlx; x++){

			if		(a->x <= b->x && a->y/4 <= b->y/4){
				ndata[2 + x*dly + y] =  adata[2 + (x+dx)	*a->msly + y + dy] |
										bdata[2 + (x)		*b->msly + y];


			}
			else if	(b->x < a->x && a->y/4 <= b->y/4){
				ndata[2 + x*dly + y] =  adata[2 + (x)		*a->msly + y + dy] |
										bdata[2 + (x+dx)	*b->msly + y];

			}
			else if	(a->x <= b->x && b->y/4 < a->y/4){
				ndata[2 + x*dly + y] =  adata[2 + (x+dx)	*a->msly + y ] |
										bdata[2 + (x)		*b->msly + y + dy];

			}
			else{
				ndata[2 + x*dly + y] =  adata[2 + (x)		*a->msly + y ] |
										bdata[2 + (x+dx)	*b->msly + y + dy];

			}

		}
	}
	free(adata);
	free(bdata);
	sendWindow(posx,posy,dlx,dly,ndata);
	free(ndata);

}
