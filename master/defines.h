/*
 * defines.h
 *
 *  Created on: Jan 5, 2018
 *      Author: root
 */

#ifndef MASTER_DEFINES_H_
#define MASTER_DEFINES_H_

//Definitions for overall Project.
#define MAXOBJECTS 32
#define MAXBLOCKS 32
#define MAXOVERLAPS 16
#define MINX 0
#define MINY 8
#define MAXX 160
#define MAXY 104

//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
//Button Masks:
#define M_U 128
#define M_D 64
#define M_L 32
#define M_R 16
#define M_A 8
#define M_B 4
#define M_P 2
#define M_S 1

//Definitions for Block.
#define DESTROY 4
#define DRAWONCE 2
#define DRAWN 1
#define NOTDRAWN 0

//Definitions for entities.
#define FRICTION 2
//Definitions of objects.
#define PLAYER 1
#define PLAYER_PROJECTILE 2
#define ENEMY 3
#define ENEMY_PROJECTILE 4
#define OBSTACLE 4
//Collision types
#define NOC 	0
#define UPPERC	1
#define LOWERC  2
#define LEFTC	3
#define RIGHTC	4

//Weapon Types
#define GUN	1
#define TRI	2
#define LAUNCHER 3
#define HEAVY 4
#define NOPPY 5
#define SHOTGUN 6
#define MACHINE 7
#define BOUNCE 8

//Bullet Types
#define BULLET 1
#define MISSILE 2
#define DISC 3
#define SHOT 4
#define BALL 5
#define EXPLOSION 6



#endif /* MASTER_DEFINES_H_ */
