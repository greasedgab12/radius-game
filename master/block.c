/*
 * block.c
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#include "block.h"
#include <stdlib.h>




Block newBlock(uint8_t x, uint8_t y, uint8_t *data){
	Block block = (Block)malloc(sizeof(struct Block_struct));
	block->x = x;
	block->y = y;
	block->lx = data[0];
	block->ly = data[1]/data[0];
	block->data = data;
	return block;
}
