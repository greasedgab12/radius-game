/*
 * game.c
 *
 *  Created on: Mar 8, 2018
 *      Author: root
 */

#include "defines.h"
#include "structure.h"
#include "entities/player.h"
#include "weapon.h"
#include "sprites.h"


GameState newGame(){
	GameState self = (GameState)malloc(sizeof(struct GameState_Struct));
	self->gunUpg =0;
	self->machineGunUpg =0;
	self->multiUpg =0;
	self->heavyUpg =0;
	self->shotGunUpg=0;
	self->noppyUpg=0;
	self->launcherUpg=0;
	self->bounceUpg=0;

	self->selWeapon =0;
	self->boughtWeapon =0;

	self->selShip =0;
	self->boughtShip =0;

	self->level =1;
	self->points =0;

	return self;
}



Object playerFromGameState(GameState state){
	Object player;
	switch(state->selShip){
		case 1:
			player = newPlayer(0,0,player_0);
			player->entity->health = 15;
			player->entity->armor =2;
			player->entity->param1=100;
			player->entity->param2=1;
			player->entity->acceleration = 3;
			player->entity->v_max = 15;

			break;
		case 2:
			player = newPlayer(0,0,player_1);
			player->entity->health = 20;
			player->entity->armor =2;
			player->entity->param1=120;
			player->entity->param2=2;
			player->entity->acceleration = 4;
			player->entity->v_max = 20;
			break;
		case 4:
			player = newPlayer(0,0,player_2);
			player->entity->health = 30;
			player->entity->armor =3;
			player->entity->param1=150;
			player->entity->param2=2;
			player->entity->acceleration = 5;
			player->entity->v_max = 20;
			break;
		case 8:
			player = newPlayer(0,0,player_3);
			player->entity->health = 50;
			player->entity->armor =4;
			player->entity->param1=200;
			player->entity->param2=3;
			player->entity->acceleration = 5;
			player->entity->v_max = 30;
			break;
		case 16:
			player = newPlayer(0,0,player_4);
			player->entity->health = 100;
			player->entity->armor =5;
			player->entity->param1=250;
			player->entity->param2=5;
			player->entity->acceleration = 5;
			player->entity->v_max = 30;
			break;
		default:
			player = newPlayer(0,0,player_0);
			player->entity->health = 10;
			player->entity->armor =1;
			player->entity->param1=100;
			player->entity->param2=1;
			player->entity->acceleration = 3;
			player->entity->v_max = 15;
			break;
	}


	player->entity->weaponA = newGun(state->gunUpg);
	switch(state->selWeapon){
		case (1<<(MACHINEGUN-1)):
			player->entity->weaponB = newMachineGun(state->machineGunUpg);
			break;
		case (1<<(MULTISHOT-1)):
			player->entity->weaponB = newMulti(state->multiUpg);
			break;
		case (1<<(HEAVY-1)):
			player->entity->weaponB = newHeavy(state->heavyUpg);
			break;
		case (1<<(SHOTGUN-1)):
			player->entity->weaponB = newShotGun(state->shotGunUpg);
			break;
		case (1<<(NOPPY-1)):
			player->entity->weaponB = newNoppy(state->noppyUpg);
			break;
		case (1<<(LAUNCHER-1)):
			player->entity->weaponB = newLauncher(state->launcherUpg);
			break;
		case (1<<(BOUNCE-1)):
			player->entity->weaponB = newBounce(state->bounceUpg);
			break;
		default:
			player->entity->weaponB =0;
	}
	return player;
}

