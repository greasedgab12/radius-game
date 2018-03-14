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

void newGun(Weapon self, uint8_t weaponState){
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

	self->projSpeed =20 + 10*speedlvl;
	self->rof = 13 - 2*roflvl;
	self->rofTime =0;

	self->fire = &fireGun;

}
void fireGun(Weapon self, Object source, Environment mainEnv){

	if(!(source->entity->energy >= self->cost)){
		return;
	}

	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		uart_putc('a');
		newProjectile(slot,BULLET);

		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx, source->y + source->ly/2 - slot->ly);
			slot->entity->v_x = self->projSpeed;
			slot->entity->a_x = self->projSpeed/5;

		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2- slot->ly);
			slot->entity->v_x = -self->projSpeed;
			slot->entity->a_x = -self->projSpeed/5;

		}

		slot->entity->armor = self->damage;
		slot->entity->health = 1;
		slot->activeState = ACTIVE;
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}


}

void newMulti(Weapon self,uint8_t weaponState){

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

}
void fireMulti(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		uint8_t i;
		for(i=0; i<self->projCount;i++){

			Object slot = getProjectileSlot(mainEnv);
			if(!slot){
				return;
			}
			newProjectile(slot, BULLET);

			if(source->type == PLAYER){
				slot->type= PLAYER_PROJECTILE;
				uint8_t offsety = source->y + source->ly- self->projCount*slot->ly/2;


				slot->setXY(slot,source->x+source->lx, offsety +slot->ly*i);
				slot->entity->v_x = self->projSpeed;
				slot->entity->a_x = self->projSpeed/5;
				if(!self->projCount%2){
					slot->entity->v_y = -10 + (20/self->projCount*i);
				}
				else{
					slot->entity->v_y = -10 + (20/self->projCount*(i+1));
				}

			}
			else if( source->type == ENEMY){
				slot->type = ENEMY_PROJECTILE;
				slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
				slot->entity->v_x = -self->projSpeed;
				slot->entity->a_x = -self->projSpeed/5;

		}

		slot->entity->armor = self->damage;
		slot->entity->health = 1;

		}
		uart_putc('d');
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}

}

void newLauncher(Weapon self,uint8_t weaponState){

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

	self->rof = 20 - 3*roflvl;

	self->fire = &fireMissile;


}
void fireMissile(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		uart_putc('c');
		newProjectile(slot, MISSILE);

		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx, source->y + source->ly/2);
			slot->entity->v_x = self->projSpeed;

		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
			slot->entity->v_x = -self->projSpeed;

		}

		slot->entity->v_x += source->entity->v_x/2;
		slot->entity->v_y += source->entity->v_y/2;

		slot->entity->armor = self->damage;
		slot->entity->health = 1;
		slot->entity->v_max = 20 +5*((self->weaponState&0b11000000)>>6);

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}

}


void newHeavy(Weapon self,uint8_t weaponState){

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

	self->rof = 25 - 2*roflvl;

	self->fire = &fireHeavy;


}
void fireHeavy(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		newProjectile(slot, BULLETHEAVY);
		uart_putc('l');
		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx, source->y + source->ly/2);
			slot->entity->v_x = self->projSpeed;
			slot->entity->a_x = self->projSpeed/5;
			source->entity->v_x-=20;

		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
			slot->entity->v_x = -self->projSpeed;
			slot->entity->a_x = -self->projSpeed/5;
			source->entity->v_x+=20;
		}


		slot->entity->armor = self->damage;
		slot->entity->health = 1;
		slot->activeState = ACTIVE;

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}
}

void newShotGun(Weapon self,uint8_t weaponState){


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

}
void fireShot(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}
		source->entity->v_x -= 20;

		uint8_t middle = source->y + source->ly/2;

		newProjectile(slot, SHOTUPPER);
		slot->type= PLAYER_PROJECTILE;
		slot->setXY(slot,source->x+source->lx, middle-slot->ly);
		slot->entity->armor = self->damage;
		slot->entity->health = self->projSpeed;
		slot->activeState = ACTIVE;



		int8_t i;

		for(i=1; i<((self->weaponState& 0b11000000)>>6) + 2; i++){
			Object slot = getProjectileSlot(mainEnv);
			if(!slot){
				return;
			}
			newProjectile(slot, SHOTMIDDLE);
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx + slot->lx*i, middle-slot->ly);
			slot->entity->armor = self->damage;
			slot->entity->health = self->projSpeed;
			slot->activeState = ACTIVE;

		}


		uart_putc('d');
		}
}

void newMachineGun(Weapon self,uint8_t weaponState){


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


}
void fireMachineGun(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;

		Object slot = getProjectileSlot(mainEnv);

		if(!slot){
			return;
		}
		newProjectile(slot, BULLET);
		uart_putc('a');
		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			uint8_t randpos;
			randpos = ((uint8_t) random())%source->ly;
			slot->setXY(slot,source->x+source->lx, source->y + randpos);
			slot->entity->v_x = self->projSpeed;
			slot->entity->a_x = self->projSpeed/5;
			source->entity->v_x-=5;

		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
			slot->entity->v_x = -self->projSpeed;
			slot->entity->a_x = -self->projSpeed/5;
			source->entity->v_x+=5;
		}


		slot->entity->armor = self->damage;
		slot->entity->health = 1;
		slot->activeState = ACTIVE;

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

		}
}


void newNoppy(Weapon self,uint8_t weaponState){

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


}
void fireDisc(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		uart_putc('b');
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}

		newProjectile(slot,DISC);

		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx, source->y + source->ly/2);
			slot->entity->v_x = self->projSpeed;
			slot->entity->a_x = self->projSpeed/5;


		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
			slot->entity->v_x = -self->projSpeed;
			slot->entity->a_x = -self->projSpeed/5;
		}
		slot->entity->v_max = 10 + self->projSpeed*5;
		slot->entity->v_y = source->entity->v_y%slot->entity->v_max;

		slot->entity->armor = self->damage;
		slot->entity->health = 1;
		slot->activeState = ACTIVE;

		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}

	}
}


void newBounce(Weapon self,uint8_t weaponState){

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


}



void fireBounce(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		uart_putc('e');
		newProjectile(slot, BALL);

		if(source->type == PLAYER){
			slot->type= PLAYER_PROJECTILE;
			slot->setXY(slot,source->x+source->lx, source->y + source->ly/2);
			slot->entity->v_x = self->projSpeed;
			slot->entity->a_x = self->projSpeed/5;


		}
		else if( source->type == ENEMY){
			slot->type = ENEMY_PROJECTILE;
			slot->setXY(slot,source->x - slot->lx, source->y + source->ly/2);
			slot->entity->v_x = -self->projSpeed;
			slot->entity->a_x = -self->projSpeed/5;
		}

		slot->entity->v_y = source->entity->v_y%slot->entity->v_max;

		slot->entity->armor = self->damage;
		slot->entity->health = 2 + ((self->weaponState&0b11000000)>>6);
		slot->activeState = ACTIVE;
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}
	}
}
void newLaser(Weapon self,uint8_t weaponState){

	self->weaponType = LASER;
	self->weaponState = weaponState;

	self->projCount = 1;

	uint8_t damagelvl, speedlvl, roflvl, uniquelvl;
	damagelvl = weaponState& 0b00000011;
	speedlvl = (weaponState& 0b00001100)>>2;
	roflvl = (weaponState& 0b00110000)>>4;
	uniquelvl = (weaponState& 0b11000000)>>6;

	self->damage = 10 + 3*damagelvl;


	self->cost = 30;

	self->projSpeed =5 + 2*speedlvl;

	self->rof = 30 - 5*roflvl;

	self->fire = &fireLaser;


}

void fireLaser(Weapon self, Object source, Environment mainEnv){
	if(!(source->entity->energy >= self->cost)){
		return;
	}
	if(((uint16_t) mainEnv->time) >= self->rofTime + self->rof){
		self->rofTime= mainEnv->time;
		Object slot = getProjectileSlot(mainEnv);
		if(!slot){
			return;
		}
		uart_putc('g');
		switch((self->weaponState& 0b11000000)>>6)
		{
			case 1:
				newProjectile(slot, LASER1);
				break;
			case 2:
				newProjectile(slot, LASER2);
				break;
			case 3:
				newProjectile(slot, LASER3);
				break;
			default:
				newProjectile(slot, LASER0);
				break;


		}

		slot->entity->health = self->projSpeed;

		slot->type= PLAYER_PROJECTILE;
		slot->setXY(slot,source->x+source->lx, source->y + source->ly/2 - slot->ly/2);
		slot->entity->v_x = self->projSpeed;
		slot->entity->a_x = self->projSpeed/5;

		slot->lx = MAXX - slot->x;
		slot->entity->armor = self->damage;
		if(source->entity->energy > self->cost){
			source->entity->energy -= self->cost;
		}
		else{
			source->entity->energy =0;
		}
	}
}
