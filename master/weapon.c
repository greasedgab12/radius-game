/*
 * weapon.c
 *
 *  Created on: Mar 7, 2018
 *      Author: root
 */
#include <stdlib.h>
#include "structure.h"
#include "defines.h"
#include "environment.h"

#include "entities/projectile.h"
#include "weapon.h"

Weapon newGun(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =15 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;
	self->rofTime =0;

	self->fire = &fireGun;

	return self;

}
void fireGun(Weapon self, Object source, Environment mainEnv){
		print("H",155,23);
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		Object b0 = newProjectile(BULLET);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			b0->setXY(b0,source->x+source->lx, source->y + source->ly/2);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projAccel;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projAccel;

		}

		b0->entity->armor = self->damage;
		b0->entity->health = 1;

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

		addObject(mainEnv, b0);




	}


}

Weapon newTri(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireTri(Weapon self, Object source, Environment mainEnv);

Weapon newMissile(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireMissile(Weapon self, Object source, Environment mainEnv);

Weapon newHeavy(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireHeavy(Weapon self, Object source, Environment mainEnv);

Weapon newShot(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireShot(Weapon self, Object source, Environment mainEnv);

Weapon newMachine(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireMachine(Weapon self, Object source, Environment mainEnv);

Weapon newDisc(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireDisc(Weapon self, Object source, Environment mainEnv);

Weapon newBounce(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = GUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =10 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	return self;

}
void fireBounce(Weapon self, Object source, Environment mainEnv);
