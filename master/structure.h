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
typedef struct Entity_Struct* Entity;
typedef struct Weapon_Struct* Weapon;
typedef struct GameState_Struct* GameState;


struct Object_Struct{

	//Position and physical object size measured in pixels.
	uint8_t x,y,lx,ly;
	const uint8_t *data;
	/**sly: Sprite length in y-direction. Length measured in pages.
	 * slx: Sprite length in x-direction.
	 * msly: Mapped Sprite length in y-direction. Length measured in pages.
	 */
	uint8_t slx,sly,msly;
	//State of the Sprite i.e. NOTDRAWN or DRAWN.
	uint8_t drawState;
	/**State of the Object:
	 * EMPTY: Object memory can be reused.
	 * INACTIVE: Object is waiting for use.
	 * ACTIVE: Object is active (thinking, colliding)
	 */
	uint8_t activeState;

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
    void (*think)(Object self, Environment env);
    uint8_t (*collide)(Object self, Object other , uint8_t cType, uint8_t iter);
};


struct Environment_struct{
	//Buttons pressed at the time of updateEnvironment.
	uint8_t buttons;
	//Time before the last call of updateEnvironment.
	uint32_t lastTime;
	//Time at the last call of updateEnvironment.
	uint32_t time;
	//Pointer to current GameState.
	GameState gameState;
	//List of all objects involved in the game.
	Object* objectList;
	//Pointer to the current active player object.
	Object player;
	//Weapons of the player.
	Weapon weaponA;
	Weapon weaponB;
	//Current level.
	uint8_t level;
	//Enemies left to destroy in the current level.
	uint8_t enemyRemaining;
	//Enemies currently on the screen.
	uint8_t enemyCount;
	//Maximum of enemies on screen.
	uint8_t enemyMax;

	//List of inactive enemies/enemies left to spawn.
	Object spawnList[4];
	//List containing the spawn time for the enemies
	uint32_t spawnDelay[4];

	/**Points gained in the current level.
	 * Value will be added to the points of the gameState after the level.
	 */
	uint16_t points;
};



struct Entity_Struct{

	//Acceleration
	int8_t a_x;
    int8_t a_y;

    //Maximum velocity
    uint8_t v_max;
    //Velocity
    int8_t v_x;
	int8_t v_y;
	//Movement remainder. Needed to allow speeds below 1 pixel per frame.
	int8_t s_x;
    int8_t s_y;

	//Variable governing entity behavior, namely the GLIDER type enemy.
	uint8_t state;

	uint8_t health;
	uint8_t maxHealth;
	uint8_t energy;
	uint8_t maxEnergy;
	uint8_t armor;
	//Additional value used differently depending on the object type.
	uint8_t param1;
};

struct Weapon_Struct{

	//Weapon damage.
	uint8_t damage;
	//Weapon energy used when firing.
	uint8_t cost;
	//Amount of projectiles fired.
	uint8_t projCount;
	//Speed value used in calculation of the actual projectile speed.
	//Might be used differently based on weapon type.
	uint8_t projSpeed;
	/**Rate of Fire variables:
	 * rofTime: Time of the last shot.
	 * rof: delay between shots. Misnomer.
	 */
	uint32_t rofTime;
	uint8_t rof;

	//Type of the Weapon. See defines.h.
	uint8_t weaponType;
	//Upgrade state of the weapon.
	uint8_t weaponState;


	void (*fire)(Weapon self, Object source, Environment env);

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
	uint8_t laserUpg;

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
