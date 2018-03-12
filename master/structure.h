/*
 * structures.h
 *
 *  Created on: Feb 26, 2018
 *      Author: root
 */

#ifndef MASTER_STRUCTURE_H_
#define MASTER_STRUCTURE_H_
#include "inttypes.h"
#include "defines.h"

typedef struct Environment_struct* Environment;
typedef struct Object_Struct* Object;
typedef struct Block_struct* Block;
typedef struct Entity_Struct* Entity;
typedef struct Weapon_Struct* Weapon;
typedef struct GameState_Struct* GameState;



struct Block_struct{
	uint8_t x,y,lx,ly;
	uint8_t blockType;
	uint8_t *data;
};

struct Object_Struct{
	//Position and object size measured in pixels.
	uint8_t x,y,lx,ly;
	const uint8_t *data;
	/**py: Page position at x-coordinate.
	 * sly: Sprite length in y-direction. Length measured in pages.
	 * slx: Sprite length in x-direction.
	 * msly: Mapped Sprite length in y-direction. Length measured in pages.
	 */
	uint8_t py,slx,sly,msly;
	//State of the Sprite i.e. NOTDRAWN or DRAWN.
	uint8_t drawState;

	//Pointer to Object data aside from Position and Graphics i.e. HP of player.
	Entity entity;
	//Type of the Object. See Objectlist for types
	uint8_t type;
	//State of the object i.e. will cause the object to be cleaned up.
	uint8_t killedBy;
	//methods
    void (*setXY)(Object self, uint8_t x, uint8_t y);
    void (*setData)(Object self, const uint8_t *sprite);

    /* Whilst move will stay the same for any type of object, think and collide differ.
     * I.e. think for the player object will access the pressed buttons and a projectile
     * will only care for the elapsed time. Similarly, collide will trigger diffrent actions
     * depending on the involved objects.
    */
    void (*think)(Object self, Environment mainEnv);
    uint8_t (*collide)(Object self, Object other , uint8_t cType, uint8_t iter);
};


struct Environment_struct{
	uint8_t buttons;
	uint32_t lastTime;
	uint32_t time;
	GameState gameState;
	Object* objectList;
	uint8_t oPos;

	Object player;
	uint8_t level;
	uint8_t enemyRemaining;
	uint8_t enemyCount;
	uint8_t enemyMax;

	Object spawnList[4];
	uint16_t spawnDelay[4];
	uint8_t spawnPos;


	uint32_t points;
};



struct Entity_Struct{

	//Velocity variables
	uint8_t acceleration;
	int8_t a_x;
    int8_t a_y;

	uint16_t v_time;
	uint8_t v_delay;

    uint8_t v_max;
    int8_t v_x;
	int8_t v_y;

	int8_t s_x;
	int8_t s_y;

	//Variables governing entity behavior.
	uint8_t state;

	uint8_t health;
	uint8_t maxHealth;
	uint8_t energy;
	uint8_t maxEnergy;
	uint8_t armor;

	uint8_t param1;
	uint8_t param2;

	Weapon weaponA;
	Weapon weaponB;
};

struct Weapon_Struct{

	uint8_t damage;
	uint8_t cost;
	uint8_t projCount;

	uint8_t projSpeed;
	uint8_t projAccel;

	uint16_t rofTime;
	uint8_t rof;

	uint8_t weaponType;
	uint8_t weaponState;


	void (*fire)(Weapon self, Object source, Environment mainEnv);

};

struct GameState_Struct{
	//Weapon upgrade variables.
	uint8_t gunUpg;
	uint8_t machineGunUpg;
	uint8_t multiUpg;
	uint8_t heavyUpg;
	uint8_t shotGunUpg;
	uint8_t noppyUpg;
	uint8_t launcherUpg;
	uint8_t bounceUpg;

	//Currently selected weapon.
	uint8_t selWeapon;
	//List of bought weapons.
	uint8_t boughtWeapon;

	//Currently selected ship.
	uint8_t selShip;
	//List of bought ships.
	uint8_t boughtShip;
	//Valid Points
	uint32_t points;
	//Current Level
	uint8_t level;
};


#endif /* MASTER_STRUCTURE_H_ */
