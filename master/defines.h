/*
8 * defines.h
 *
 *  Created on: Jan 5, 2018
 *      Author: root
 */

#ifndef MASTER_DEFINES_H_
#define MASTER_DEFINES_H_

//Definitions for overall Project.
#define MAXENEMIES 6
#define MAXPROJECTILES 8
#define MAXEFFECTS 3
#define MAXOBJECTS 1+MAXENEMIES+MAXPROJECTILES+MAXEFFECTS
#define MAXOVERLAPS 16
#define MINX 0
#define MINY 9
#define MAXX 160
#define MAXY 104


//Definitions for activity state of Objects
#define EMPTY 0
#define INACTIVE 1
#define ACTIVE 2

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
#define BORDER 1
#define PLAYER 2
#define PLAYER_PROJECTILE 3
#define ENEMY 4
#define ENEMY_PROJECTILE 5
#define OBSTACLE 6
//Collision types
#define NOC 	0
#define UPPERC	1
#define LOWERC  2
#define LEFTC	3
#define RIGHTC	4

//Weapon Types
#define GUN	1
#define MACHINEGUN 2
#define MULTISHOT 3
#define HEAVY 4
#define SHOTGUN 5
#define NOPPY 6
#define LAUNCHER 7
#define BOUNCE 8
#define LASER 9

//Bullet Types
#define BULLET 1
#define MISSILE 2
#define DISC 3
#define SHOTUPPER 4
#define SHOTLOWER 5
#define SHOTMIDDLE 6
#define BALL 7
#define BULLETHEAVY 9
#define BULLETENEMY 10
#define LASER0 11
#define LASER1 12
#define LASER2 13
#define LASER3 14

//Enemy Types
#define FLOATER 0
#define GLIDER 1
#define TRACKER 2
#define SHOOTER 3

//Spawn Types
#define SINGLE 0
#define DOUBLE 1
#define TRIPLE 2
#define TRIPLECHAIN 3
#define QUADCHAIN 4

//Effect Types
#define EXPLOSION 0
#define COLLISION 1
#define IMPACT 2

#endif /* MASTER_DEFINES_H_ */
