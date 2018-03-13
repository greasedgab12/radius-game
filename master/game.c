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
#include "entities/enemy.h"
#include "weapon.h"
#include "display.h"
#include "sprite.h"
#include "sprites.h"
#include "environment.h"
#include "char.h"
#include "buttons.h"

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



Object getPlayerFromGameState(GameState state){
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
void setSpawnList(Environment env){
	print("SPAWN",0,2);
	uint8_t level = env->level;
	uint8_t delay=120 - (5*level)%90;
	uint8_t strength = 2*level;
	uint8_t value;

	uint8_t health,armor,speed;

	value = (uint8_t)(random())%(strength+1);
	strength -= value;
	health =  1 + value;

	value = (uint8_t)(random())%(strength+1);
	strength -= value;
	armor = 1 + value;

	value = (uint8_t)(random())%(strength+1);
	strength -= value;
	speed = 6+ FRICTION + value;
	
	uint8_t* shipSprite = enemy_0;
	uint8_t shipType = 0;
	uint8_t spawnType = 0;

	value = (uint8_t)(random())%5;
	if(value ==0){
		shipSprite = enemy_0;
	}
	else if(value ==1){
		shipSprite = enemy_1;
	}
	else if(value ==2){
		shipSprite = enemy_2;
	}
	else if(value ==3){
		shipSprite = enemy_3;
	}
	else if(value ==4){
		shipSprite = enemy_4;
	}

	shipType = (level>4?1:0) + (level>6?1:0) + (level >8?1:0);
	shipType = (uint8_t)(random())%(shipType+1);


	uint8_t param2 =0;
	if(shipType == GLIDER){
		param2 = (uint8_t)(random())%(50)+50;
	}
	else if(shipType == SHOOTER){
		param2 = 250 - random()%(50);
	}

	spawnType = (level>3?1:0) + (level>6?1:0) + (level>9?1:0) + (level>12?1:0) + (level>15?1:0);
	spawnType = (uint8_t)(random())%(spawnType+1) ;



	if(spawnType == SINGLE){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY) + MINY;
		env->spawnList[0] = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		env->spawnDelay[0] = env->time + delay;
	}

	else if(spawnType == DOUBLE){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY - 32) + MINY + 10;

		Object enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y -16);
		env->spawnList[0] = enemy;
		env->spawnDelay[0] = env->time + delay;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y +16);
		env->spawnList[1] = enemy;
		env->spawnDelay[1] = env->time + delay;


	}
	else if(spawnType == TRIPLE){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY - 32) + MINY + 32;

		Object enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y -32);
		env->spawnList[0] = enemy;
		env->spawnDelay[0] = env->time + delay;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y -16);
		env->spawnList[1] = enemy;
		env->spawnDelay[1] = env->time + delay;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[2] = enemy;
		env->spawnDelay[2] = env->time + delay;



	}
	else if(spawnType == TRIPLECHAIN){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY) + MINY ;

		Object enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y);
		env->spawnList[0] = enemy;
		env->spawnDelay[0] = env->time + delay;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[1] = enemy;
		env->spawnDelay[1] = env->time + delay + 45;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[2] = enemy;
		env->spawnDelay[2] = env->time + delay + 90;

	}
	else if(spawnType == QUADCHAIN){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY) + MINY ;

		Object enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y);
		env->spawnList[0] = enemy;
		env->spawnDelay[0] = env->time + delay;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[1] = enemy;
		env->spawnDelay[1] = env->time + delay + 45;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[2] = enemy;
		env->spawnDelay[2] = env->time + delay + 90;

		enemy = getEnemyByType(shipType, shipSprite, health, armor, speed,yPosition,param2);
		enemy->setXY(enemy,enemy->x, enemy->y );
		env->spawnList[3] = enemy;
		env->spawnDelay[3] = env->time + delay + 135;

	}
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
					env->enemyRemaining--;
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
	static uint8_t lastHP=0, lastE=0, lastP=0;
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

			sendWindow(46,0,42,2,bar1);
			sendWindow(46,0,1 + value%41, 2, bar0);
			flush_sprite(energybar_0);
			flush_sprite(energybar_1);
			lastE = player->entity->energy;
		}

		if(mainEnv->points != lastP){
			lastP = mainEnv->points;
			printN(mainEnv->points,120,0);
		}

	}




}

