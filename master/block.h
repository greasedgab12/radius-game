#ifndef MASTER_BLOCK_H_
#define MASTER_BLOCK_H_

#define DESTROY 3
#define DRAWONCE 2
#define DRAWN 1
#define NOTDRAWN 0

#include <inttypes.h>
#include <avr/pgmspace.h>



typedef struct Block_struct* Block;

struct Block_struct{
	uint8_t x,y,lx,ly;
	uint8_t blocktype;
	uint8_t *data;
};

/* newBlock: Create new Block from existing Data ( in case of overlap).
 * uint8_t x: x coordinate of new Block.
 * uint8_t y: y coordinate of new Block.
 * uint8_t *data: bitmap of new Block.
 * return Block: newly allocated Block from parameters.
*/
Block newBlock(uint8_t x, uint8_t y, uint8_t *data);

/* releaseBlock: free memory of a block.
 * Block instance: Block to be freed.
 */
void releaseBlock(Block instance);

/* checkBlockCollision: Check list of Blocks for collisions.
 * Block* blockList: List of Blocks for collision check.
 * uint8_t length: Length of that list.
 * return Block*: List of fused overlapping Blocks terminated with a nullptr!
*/
Block* checkBlockCollision(Block* blockList, uint8_t length);

/* getOverlap: Calculate and allocate overlapping block.
 * Block b1: first Block
 * Block b2: second Block
 * return Block: Resulting Block with the dimensions of the overlapping area.
 */
Block getOverlap(Block b1, Block b2);

/* drawBlock: Draws Block with either page or window function, depending on size of block.
 * Block instance: Block to draw.
 */
void drawBlock(Block instance);

#endif /* MASTER_BLOCK_H_ */
