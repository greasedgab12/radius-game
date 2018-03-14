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

void newGame(GameState self){
	self->gunUpg =0;
	self->machineGunUpg =0;
	self->multiUpg =0;
	self->heavyUpg =0;
	self->shotGunUpg=0;
	self->noppyUpg=0;
	self->launcherUpg=0;
	self->bounceUpg=0;
	self->laserUpg= 255;

	self->selWeapon = LASER-1;
	self->boughtWeapon =255;

	self->selShip =1;
	self->boughtShip =1;

	self->level =1;
	self->points =1000;


}



void getPlayerFromGameState(Environment env){
	GameState state = env->gameState;
	Object player = env->player;
	player->killedBy =0;
	switch(state->selShip){
		case 1:
			newPlayer(player,0,0,player_0);
			player->entity->health = 15;
			player->entity->maxHealth = 15;
			player->entity->maxEnergy=100;
			player->entity->armor =2;
			player->entity->param1=1;
			player->entity->v_max = 20;

			break;
		case 2:
			newPlayer(player,0,0,player_1);
			player->entity->health = 20;
			player->entity->maxHealth = 20;
			player->entity->maxEnergy=120;
			player->entity->armor =2;
			player->entity->param1=2;
			player->entity->v_max = 25;
			break;
		case 3:
			newPlayer(player,0,0,player_2);
			player->entity->health = 40;
			player->entity->maxHealth = 40;
			player->entity->maxEnergy=150;
			player->entity->armor =3;
			player->entity->param1=2;
			player->entity->v_max = 30;
			break;
		case 4:
			newPlayer(player,0,0,player_3);
			player->entity->health = 100;
			player->entity->maxHealth = 100;
			player->entity->maxEnergy=200;
			player->entity->armor =4;
			player->entity->param1=3;
			player->entity->v_max = 35;
			break;
		default:
			newPlayer(player,0,0,player_0);
			player->entity->health = 10;
			player->entity->maxHealth = 10;
			player->entity->maxEnergy=100;
			player->entity->armor =1;
			player->entity->param1=1;
			player->entity->v_max = 15;
			break;
	}
	newGun(env->weaponA,state->gunUpg);
	switch(state->selWeapon){
		case ((MACHINEGUN-1)):
			newMachineGun(env->weaponB,state->machineGunUpg);
			break;
		case ((MULTISHOT-1)):
			newMulti(env->weaponB,state->multiUpg);
			break;
		case ((HEAVY-1)):
			newHeavy(env->weaponB,state->heavyUpg);
			break;
		case ((SHOTGUN-1)):
			newShotGun(env->weaponB,state->shotGunUpg);
			break;
		case ((NOPPY-1)):
			newNoppy(env->weaponB,state->noppyUpg);
			break;
		case (LAUNCHER-1):
			newLauncher(env->weaponB,state->launcherUpg);
			break;
		case ((BOUNCE-1)):
			newBounce(env->weaponB,state->bounceUpg);
			break;
		case ((LASER-1)):
			newLaser(env->weaponB,state->laserUpg);
			break;
		default:
			newGun(env->weaponB,state->gunUpg);
			env->weaponB->cost =255;
			break;
	}

}
//Only execute setSpawn when SpawnList is empty;
void setSpawnList(Environment env){
	uint8_t level = env->level;
	uint8_t delay=80 - (2*level)%45;
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
	speed = 10+ FRICTION + value;
	
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
		Object slot;
		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[0] = slot;
			env->spawnDelay[0] = env->time + delay;
		}
	}

	else if(spawnType == DOUBLE){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY - 32) + MINY + 10;
		Object slot;
		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[0] = slot;
			env->spawnDelay[0] = env->time + delay;
		}


		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[1] = slot;
			env->spawnDelay[1] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y +16);
		}



	}
	else if(spawnType == TRIPLE){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY - 32) + MINY + 32;
		Object slot;
		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[0] = slot;
			env->spawnDelay[0] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y -32);

		}
		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[1] = slot;
			env->spawnDelay[1] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y -16);

		}

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[2] = slot;
			env->spawnDelay[2] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y);

		}



	}
	else if(spawnType == TRIPLECHAIN){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY) + MINY ;
		Object slot;

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[0] = slot;
			env->spawnDelay[0] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y);

		}

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[1] = slot;
			env->spawnDelay[1] = env->time + delay +10;
			slot->setXY(slot,slot->x, slot->y);

		}

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[2] = slot;
			env->spawnDelay[2] = env->time + delay +20;
			slot->setXY(slot,slot->x, slot->y);

		}

	}
	else if(spawnType == QUADCHAIN){
		uint8_t yPosition=0;
		yPosition = random()%(MAXY - MINY) + MINY ;

		Object slot;

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[0] = slot;
			env->spawnDelay[0] = env->time + delay;
			slot->setXY(slot,slot->x, slot->y);

		}

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[1] = slot;
			env->spawnDelay[1] = env->time + delay +10;
			slot->setXY(slot,slot->x, slot->y);

		}

		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[2] = slot;
			env->spawnDelay[2] = env->time + delay +20;
			slot->setXY(slot,slot->x, slot->y);

		}
		slot = getEnemySlot(env);
		if(slot){
			getEnemyByType(slot,shipType, shipSprite, health, armor, speed,yPosition,param2);
			slot->activeState=INACTIVE;
			env->spawnList[2] = slot;
			env->spawnDelay[2] = env->time + delay +30;
			slot->setXY(slot,slot->x, slot->y);

		}
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
					env->spawnList[i]->activeState = ACTIVE;
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

void drawHud(uint8_t health,uint8_t maxHealth, uint8_t energy,uint8_t maxEnergy, uint32_t points){
	static uint8_t lastHP=0, lastE=0, lastP=0;

	if((health != lastHP)){
		uint8_t *bar0 = load_sprite(healthbar_0);
		uint8_t *bar1 = load_sprite(healthbar_1);
		uint16_t value = ((maxHealth-health)*100)/maxHealth;
		value = (value*40)/100;
		sendWindow(0,0,42,2,bar1);
		sendWindow(0,0,1 + value, 2, bar0);
		flush_sprite(bar0);
		flush_sprite(bar1);
		lastHP = health;
	}


	if((energy != lastE)){
		uint8_t *bar0 = load_sprite(energybar_0);
		uint8_t *bar1 = load_sprite(energybar_1);
		uint16_t value = ((maxEnergy-energy)*100)/maxEnergy;
		value = (value*40)/100;

		sendWindow(46,0,42,2,bar1);
		sendWindow(46,0,1 + value%41, 2, bar0);
		flush_sprite(bar0);
		flush_sprite(bar1);
		lastE = energy;
	}

	if(points != lastP){
		lastP = points;
		printN(points,120,0);
	}

}






