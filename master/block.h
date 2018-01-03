#ifndef MASTER_BLOCK_H_
#define MASTER_BLOCK_H_

#include <inttypes.h>
#include <avr/pgmspace.h>

typedef struct Block_struct* Block;

struct Block_struct{
	uint8_t x,y,lx,ly;
	uint8_t *data;
};

//Create new Block from existing Data ( in case of overlap).
Block newBlock(uint8_t x, uint8_t y, uint8_t *data);


#endif /* MASTER_BLOCK_H_ */
