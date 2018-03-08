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
#include "uart.h"


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

	if(!(source->entity->energy >= self->cost)){
		return;
	}

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

Weapon newMulti(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = MULTI;
	self->weaponState = weaponState;



	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl 	= (weaponState& 0b00000011);
	speedlvl 	= (weaponState& 0b00001100)>>2;
	roflvl 		= (weaponState& 0b00110000)>>4;
	uniquelvl 	= (weaponState& 0b11000000)>>6;

	self->projCount = 3 +uniquelvl;

	self->damage = 3 + 2*damagelvl;


	self->cost = 10 + uniquelvl;

	self->projSpeed =30 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 60 - 5*roflvl;

	self->fire = &fireMulti;

	return self;

}
void fireMulti(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		uint8_t i;
		for(i=0; i<self->projCount;i++){


		Object b0 = newProjectile(BULLET);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			uint8_t offsety = source->y + source->ly- self->projCount*b0->ly/2;


			b0->setXY(b0,source->x+source->lx, offsety +b0->ly*i);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projAccel;
			if(!self->projCount%2){
				b0->entity->v_y = -5 + (10/self->projCount*i);
			}
			else{
				b0->entity->v_y = -5 + (10/self->projCount*(i+1));
			}

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projAccel;

		}

		b0->entity->armor = self->damage;
		b0->entity->health = 1;



		addObject(mainEnv, b0);
		}

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}

}

Weapon newLauncher(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = LAUNCHER;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 5 + 5*damagelvl;


	self->cost = 10;

	self->projSpeed =20 + 10*speedlvl;
	self->projAccel =1 + 2*uniquelvl;

	self->rof = 70 - 5*roflvl;

	self->fire = &fireMissile;

	return self;

}
void fireMissile(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		Object b0 = newProjectile(MISSILE);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			b0->setXY(b0,source->x+source->lx, source->y + source->ly/2);
			b0->entity->v_x = self->projSpeed;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;

		}

		b0->entity->v_x += source->entity->v_x/2;
		b0->entity->v_y += source->entity->v_y/2;

		b0->entity->armor = self->damage;
		b0->entity->health = 1;
		b0->entity->acceleration = self->projAccel;
		b0->entity->v_max = 20 +5*((self->weaponState&0b00001100)>>2);

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

		addObject(mainEnv, b0);




	}

}


Weapon newHeavy(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = HEAVY;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 10 + 10*damagelvl;


	self->cost = 10 - uniquelvl;

	self->projSpeed =30 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 60 - 5*roflvl;

	self->fire = &fireHeavy;

	return self;

}
void fireHeavy(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		Object b0 = newProjectile(BULLETHEAVY);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			b0->setXY(b0,source->x+source->lx, source->y + source->ly/2);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projAccel;
			source->entity->v_x-=20;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projAccel;
			source->entity->v_x+=20;
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
/**
Weapon newShot(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = SHOTGUN;
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
	self->fire = &fireShot;

	return self;

}
//ToDo: Implement shotgun with new projectile type
void fireShot(Weapon self, Object source, Environment mainEnv);
**/

Weapon newMachine(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = MACHINE;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 2 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =20 + 10*speedlvl;
	self->projAccel =0;

	self->rof = 16 - 3*roflvl;

	self->fire = &fireMachine;

	return self;

}
void fireMachine(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		Object b0 = newProjectile(BULLET);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			uint8_t randpos;
			randpos = ((uint8_t) random())%source->ly;
			b0->setXY(b0,source->x+source->lx, source->y + randpos);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projAccel;
			source->entity->v_x-=5;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projAccel;
			source->entity->v_x+=5;
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


Weapon newNoppy(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = NOPPY;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 1 + damagelvl;


	self->cost = 4 - uniquelvl;

	self->projSpeed =20 + 6*speedlvl;
	self->projAccel =0;

	self->rof = 30 - 5*roflvl;

	self->fire = &fireDisc;

	return self;

}
void fireDisc(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		uart_putc('a');

		Object b0 = newProjectile(DISC);

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

		b0->entity->v_y = source->entity->v_y%b0->entity->v_max;

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

//void fireBounce(Weapon self, Object source, Environment mainEnv);
