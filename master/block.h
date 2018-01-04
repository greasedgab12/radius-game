#ifndef MASTER_BLOCK_H_
#define MASTER_BLOCK_H_

#include <inttypes.h>
#include <avr/pgmspace.h>

typedef struct Block_struct* Block;

struct Block_struct{
	uint8_t x,y,lx,ly;
	uint8_t *data;
};

/* newBlock: Create new Block from existing Data ( in case of overlap).
 * uint8_t x: x coordinate of new Block.
 * uint8_t y: y coordinate of new Block.
 * uint8_t *data: bitmap of new Block.
 * return Block: newly allocated Block from parameters.
*/
Block newBlock(uint8_t x, uint8_t y, uint8_t *data);

/* checkBlockCollision: Check list of Blocks for collisions.
 * Block* blockList: List of Blocks for collision check.
 * uint8_t length: Length of that list.
 * return Block*: List of fused overlapping Blocks.
*/
Block* checkBlockCollision(Block* blockList, uint8_t length);

#endif /* MASTER_BLOCK_H_ */
