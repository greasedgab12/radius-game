/*
 * game.c
 *
 *  Created on: Mar 8, 2018
 *      Author: root
 */

#include <stdlib.h>
#include "defines.h"
#include "structure.h"
#include "entities/player.h"
#include "weapon.h"
#include "display.h"
#include "sprite.h"
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
	self->laserUpg= 0;

	self->selWeapon =0;
	self->boughtWeapon =0b0;

	self->selShip =1;
	self->boughtShip =0b0001;

	self->level =1;
	self->points =1000;

	return self;
}



Object playerFromGameState(GameState state){
	Object player;
	switch(state->selShip){
		case 1:
			player = newPlayer(0,0,player_0);
			player->entity->health = 15;
			player->entity->maxHealth = 15;
			player->entity->maxEnergy=100;
			player->entity->armor =2;
			player->entity->param1=1;
			player->entity->param2=0;
			player->entity->acceleration = 3;
			player->entity->v_max = 15;

			break;
		case 2:
			player = newPlayer(0,0,player_1);
			player->entity->health = 20;
			player->entity->maxHealth = 20;
			player->entity->maxEnergy=120;
			player->entity->armor =2;
			player->entity->param1=2;
			player->entity->param2=0;
			player->entity->acceleration = 4;
			player->entity->v_max = 20;
			break;
		case 4:
			player = newPlayer(0,0,player_2);
			player->entity->health = 40;
			player->entity->maxHealth = 40;
			player->entity->maxEnergy=150;
			player->entity->armor =3;
			player->entity->param1=2;
			player->entity->param2=0;
			player->entity->acceleration = 5;
			player->entity->v_max = 20;
			break;
		case 8:
			player = newPlayer(0,0,player_3);
			player->entity->health = 100;
			player->entity->maxHealth = 100;
			player->entity->maxEnergy=200;
			player->entity->armor =4;
			player->entity->param1=3;
			player->entity->param2=0;
			player->entity->acceleration = 5;
			player->entity->v_max = 30;
			break;
		default:
			player = newPlayer(0,0,player_0);
			player->entity->health = 10;
			player->entity->maxHealth = 10;
			player->entity->maxEnergy=100;
			player->entity->armor =1;
			player->entity->param1=1;
			player->entity->param2=0;
			player->entity->acceleration = 3;
			player->entity->v_max = 15;
			break;
	}


	player->entity->weaponA = newGun(state->gunUpg);
	switch(state->selWeapon){
		case (1<<(MACHINEGUN-2)):
			player->entity->weaponB = newMachineGun(state->machineGunUpg);
			break;
		case (1<<(MULTISHOT-2)):
			player->entity->weaponB = newMulti(state->multiUpg);
			break;
		case (1<<(HEAVY-2)):
			player->entity->weaponB = newHeavy(state->heavyUpg);
			break;
		case (1<<(SHOTGUN-2)):
			player->entity->weaponB = newShotGun(state->shotGunUpg);
			break;
		case (1<<(NOPPY-2)):
			player->entity->weaponB = newNoppy(state->noppyUpg);
			break;
		case (1<<(LAUNCHER-2)):
			player->entity->weaponB = newLauncher(state->launcherUpg);
			break;
		case (1<<(BOUNCE-2)):
			player->entity->weaponB = newBounce(state->bounceUpg);
			break;
		default:
			player->entity->weaponB =0;
	}
	return player;
}
//Only execute setSpawn when SpawnList is empty;
void setSpawn(Environment env){
	uint8_t level = env->level;
	uint8_t SpawnDelay=120 - (5*level)%90;
	uint8_t strength = 2*level;
	uint8_t value;

	uint8_t health,armor,speed;

	value = random()%strength;
	strength -= value;
	health = value;

	value = random()%strength;
	strength -= value;
	armor = value;

	value = random()%strength;
	strength -= value;
	speed = value;
	
	uint8_t shipTypes;
	shipTypes = level>4 + level>6 + level >8;
	shipTypes = random()%shipTypes;
	uint8_t spawnTypes;
	spawnTypes = level>3 + level>6 + level>9 + level>12 + level>15;
	spawnTypes = random()%spawnTypes;
	if(shipTypes == 0);
	










}

uint8_t isSpawnListEmpty(Environment env){
	uint8_t i;
	for(i=0; i<4;i++){
		if(env->spawnList[i]){
			return 0;
		}
	}
	return 1;
}

void getNextEnemy(Environment env){
	uint8_t i;
	for(i=0; i<4;i++){
		if(env->enemyCount < env->enemyMax){

			if(env->spawnList[i]){
				if(env->time > env->spawnDelay[i]){
					addObject(env, env->spawnList[i]);
					env->enemyCount++;
					env->spawnList[i]  =0;
					env->spawnDelay[i] =0;
				}
			}
		}
	}




}






uint16_t getPoints(Environment mainEnv, Object enemy){

	return mainEnv->gameState->level*(enemy->entity->health*enemy->entity->armor) +enemy->entity->v_max/2;

}

void drawHud(Environment mainEnv){
	static uint8_t lastHP=0, lastE=0;
	Object player = mainEnv->player;

	if(player){
		if((player->entity->health != lastHP)){
			uint8_t *bar0 = load_sprite(healthbar_0);
			uint8_t *bar1 = load_sprite(healthbar_1);
			uint16_t value = ((player->entity->maxHealth-player->entity->health)*100)/player->entity->maxHealth;
			value = (value*40)/100;
			sendWindow(0,0,42,2,bar1);
			sendWindow(0,0,1 + value, 2, bar0);
			flush_sprite(healthbar_0);
			flush_sprite(healthbar_1);
			lastHP = player->entity->health;
		}


		if((player->entity->energy != lastE)){
			uint8_t *bar0 = load_sprite(energybar_0);
			uint8_t *bar1 = load_sprite(energybar_1);
			uint16_t value = ((player->entity->maxEnergy-player->entity->energy)*100)/player->entity->maxEnergy;
			value = (value*40)/100;

			printN(value,46,2);
			sendWindow(46,0,42,2,bar1);
			sendWindow(46,0,1 + value%41, 2, bar0);
			flush_sprite(energybar_0);
			flush_sprite(energybar_1);
			lastE = player->entity->energy;
		}

	}




}

