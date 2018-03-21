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
#define MINX 0
#define MINY 9
#define MAXX 160
#define MAXY 104


//Definitions for activity state of Objects
#define EMPTY 0
#define INACTIVE 1
#define ACTIVE 2

//Buttons:
#define B_UP (!(PIND & (1<<2)))
#define B_DOWN (!(PIND & (1<<3)))
#define B_LEFT (!(PIND & (1<<4)))
#define B_RIGHT (!(PIND & (1<<5)))
#define B_B (!(PIND & (1<<6)))
#define B_A (!(PIND & (1<<7)))

#define B_PAUSE (!(PINC & (1<<4)))
#define B_SELECT (!(PINC & (1<<5)))

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

//Definitions for drawing of Objects.
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
#define BULLETHEAVY 8
#define BULLETENEMY 9
#define LASER0 10
#define LASER1 11
#define LASER2 12
#define LASER3 13

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


#endif /* MASTER_DEFINES_H_ */
