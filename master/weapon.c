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


	self->cost = 5 - 1*uniquelvl;

	self->projSpeed =15 + 10*speedlvl;

	self->rof = 15 - 2*roflvl;
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
			b0->setXY(b0,source->x+source->lx, source->y + source->ly/2 - b0->ly);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projSpeed/5;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2- b0->ly);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;

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

	self->weaponType = MULTISHOT;
	self->weaponState = weaponState;



	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl 	= (weaponState& 0b00000011);
	speedlvl 	= (weaponState& 0b00001100)>>2;
	roflvl 		= (weaponState& 0b00110000)>>4;
	uniquelvl 	= (weaponState& 0b11000000)>>6;

	self->projCount = 3 +uniquelvl;

	self->damage = 3 + 2*damagelvl;


	self->cost = 50 + 10*uniquelvl;

	self->projSpeed =30 + 10*speedlvl;

	self->rof = 30 - 2*roflvl;

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
			b0->entity->a_x = self->projSpeed/5;
			if(!self->projCount%2){
				b0->entity->v_y = -10 + (20/self->projCount*i);
			}
			else{
				b0->entity->v_y = -10 + (20/self->projCount*(i+1));
			}

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;

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


	self->cost = 40;

	self->projSpeed =20 + 10*speedlvl;

	self->rof = 35 - 3*roflvl;

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
		b0->entity->v_max = 20 +5*((self->weaponState&0b11000000)>>6);

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


	self->cost = 60 - 5*uniquelvl;

	self->projSpeed =30 + 10*speedlvl;

	self->rof = 30 - 2*roflvl;

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
			b0->entity->a_x = self->projSpeed/5;
			source->entity->v_x-=20;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;
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

Weapon newShotGun(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = SHOTGUN;
	self->weaponState = weaponState;

	self->projCount = 5;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 10 + 10*damagelvl;


	self->cost = 40;

	self->projSpeed =8 + 2*speedlvl;

	self->rof = 30 - 3*roflvl;

	self->fire = &fireShot;

	return self;
}
//ToDo: Implement shotgun with new projectile type
void fireShot(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}
		source->entity->v_x -= 20;

		uint8_t middle = source->y + source->ly/2;

		Object bu = newProjectile(SHOTUPPER);
		bu->type= PLAYER_PROJECTILE;
		bu->setXY(bu,source->x+source->lx, middle-bu->ly);
		bu->entity->armor = self->damage;
		bu->entity->health = self->projSpeed;

		addObject(mainEnv, bu);


		int8_t i;

		for(i=1; i<((self->weaponState& 0b11000000)>>6) + 2; i++){
			printN(i,32,2);
			Object bu = newProjectile(SHOTMIDDLE);
			bu->type= PLAYER_PROJECTILE;
			bu->setXY(bu,source->x+source->lx + bu->lx*i, middle-bu->ly);
			bu->entity->armor = self->damage;
			bu->entity->health = self->projSpeed;
			addObject(mainEnv, bu);

		}



		}
}

Weapon newMachineGun(uint8_t weaponState){
	Weapon self = (Weapon)malloc(sizeof(struct Weapon_Struct));

	self->weaponType = MACHINEGUN;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 2 + damagelvl;


	self->cost = 20 - 3*uniquelvl;

	self->projSpeed =20 + 10*speedlvl;

	self->rof = 8 - 1*roflvl;

	self->fire = &fireMachineGun;

	return self;

}
void fireMachineGun(Weapon self, Object source, Environment mainEnv){
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
			b0->entity->a_x = self->projSpeed/5;
			source->entity->v_x-=5;

		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;
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


	self->cost = 20 - uniquelvl;

	self->projSpeed =20 + 6*speedlvl;

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
			b0->entity->a_x = self->projSpeed/5;


		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;
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

	self->damage = 3 + 2*damagelvl;


	self->cost = 40;

	self->projSpeed =15 + 10*speedlvl;

	self->rof = 50 - 5*roflvl;

	self->fire = &fireBounce;

	return self;

}



void fireBounce(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		uart_putc('a');

		Object b0 = newProjectile(BALL);

		if(source->type == PLAYER){
			b0->type= PLAYER_PROJECTILE;
			b0->setXY(b0,source->x+source->lx, source->y + source->ly/2);
			b0->entity->v_x = self->projSpeed;
			b0->entity->a_x = self->projSpeed/5;


		}
		else if( source->type == ENEMY){
			b0->type = ENEMY_PROJECTILE;
			b0->setXY(b0,source->x - b0->lx, source->y + source->ly/2);
			b0->entity->v_x = -self->projSpeed;
			b0->entity->a_x = -self->projSpeed/5;
		}

		b0->entity->v_y = source->entity->v_y%b0->entity->v_max;

		b0->entity->armor = self->damage;
		b0->entity->health = 2 + ((self->weaponState&0b11000000)>>6);

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

		addObject(mainEnv, b0);
		}
}
