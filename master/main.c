#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <entities/general.h>
#include <entities/projectile.h>
#include <entity.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "timer.h"
#include "buttons.h"

#include "char.h"
#include "display.h"
#include "object.h"
#include "defines.h"
#include "environment.h"
#include "entities/player.h"
#include "entities/general.h"
#include "entities/enemy.h"
#include "weapon.h"
#include "game.h"
#include "sprites.h"

#include "sprite.h"
#include "menu.h"


void init();




void titleScreen(Environment env){

	drawTitleScreen();
	uint16_t time = env->time;
	uint8_t state =0;

	while(!(env->buttons == M_S)){
		updateEnvironment(env);
		if(time + 60 < env->time){
			time = env->time;
			if(state){
				print("Press START",46,18);
				state=0;
			}
			else{
				print("           ",46,18);
				state=1;
			}
		}
	}

	displayClear();


}

void displayLevel(Environment env){
	print("LEVEL ",63,10);
	printN(env->level,99,10);
	_delay_ms(2000);

	displayClear();
}
void displayStart(Environment env){
	print("READY",66,10);
	_delay_ms(1000);
	print("START",66,14);
	_delay_ms(1000);
	displayClear();
}
void displayFinished(Environment env){
	print("LEVEL ",63,10);
	printN(env->level,99,10);
	print("CLEARED",60,12);
	print("POINTS:",50,16);
	printN(env->gameState->points,100,16);
	_delay_ms(3000);
	displayClear();
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}



int main(void)
{
	init();



	uint8_t i,j=0;
	//Environment Initialization
    //Environment is persistend throughout the entire runtime.
    Environment env = newEnvironment();
    env->gameState = newGame();
	sei();
	/**
    titleScreen(env);
	_delay_ms(200);
    //display_mainmenu();
	**/
	//ToDo: Couple gameState with selection in MainMenu.


    //Game loop
    while(1){
    	//Reloading the player object.
    	env->gameState->selShip = 0b00001000;
		env->gameState->selWeapon = 0b00000001;
		env->gameState->machineGunUpg = 255;
		env->gameState->gunUpg = 255;

    	env->player = getPlayerFromGameState(env);
		env->player->setXY(env->player,0, (MAXY - MINY)/2 -env->player->ly/2);

		env->gameState->level = 20;
		env->level = env->gameState->level;
		addObject(env, env->player);

		env->enemyRemaining = 4 + 2*env->level;
		env->enemyMax =1 + (env->level/2<5?env->level/2:5);

		updateEnvironment(env);
		/**
		displayLevel(env);
		displayStart(env);
		**/
		//Prevent the games entities from thinking too much.



		updateEnvironment(env);
		//Force redraw of HUD
		drawHud(0,1,0,1,0);
		while(env->enemyRemaining || env->enemyCount){
			printN(freeRam(),0,2);
			printN(env->player->entity->weaponB,0,4);
			//Update Environment variables.
			updateEnvironment(env);
			drawHud(env->player->entity->health,env->player->entity->maxHealth,env->player->entity->energy,env->player->entity->maxEnergy, env->points);



			if(env->enemyCount < env->enemyMax && env->enemyRemaining){
				if(isSpawnListEmpty(env)){
					setSpawnList(env);
				}
				else{
					getNextEnemy(env);
				}
			}

			//For each passed frame execute think of each object.

			for(i=0; i<env->time-(env->lastTime); i++){
				for(j=0; j<env->oPos; j++){
					env->objectList[j]->think(env->objectList[j], env);
				}
			}

			//printN(env->time, 64,0);
			//Clean up dead objects and draw alive ones if they haven't been drawn already.
			for(i=0; i<env->oPos; i++){
				if(env->objectList[i]->killedBy==0){
					if(env->objectList[i]->drawState == NOTDRAWN){
						drawObject(env->objectList[i]);
					}
				}
				else{
					sendWindow(env->objectList[i]->x,env->objectList[i]->y/4,
							env->objectList[i]->slx, env->objectList[i]->msly,0);
				}
			}

			//Remove dead objects from list.
			for(i=0; i<env->oPos; i++){
				if(env->objectList[i]->killedBy!=0){
					if(env->objectList[i]->type == ENEMY){
						//If dead Object is of type ENEMY decrease enemyCount, to allow more enemies to spawned.
						env->enemyCount--;
						/**Should the kill owner be the PlAYER increase Points,
						 * else increase enemyRemaining so that the level only ends
						 * on all enemies destroyed by the player.
						 */
						if(env->objectList[i]->killedBy == PLAYER){
							env->points+=getPoints(env,env->objectList[i]);
						}
						else{
							env->enemyRemaining++;
						}
					}
					removeObject(env, env->objectList[i]);

				}
			}

			//Check, wether drawn sprites overlap one another and draw overlapping parts again.
			//Skip if frames are dropped.
			if(env->time -env->lastTime < 3){
				checkMappedSpriteCollision(env->objectList,env->oPos);
			}
			//Update drawState of every object.
			for(i=0; i<env->oPos; i++){
				if(env->objectList[i]->drawState== NOTDRAWN){
					env->objectList[i]->drawState = DRAWN;
				}
				if(env->objectList[i]->drawState == DRAWONCE){
					env->objectList[i]->drawState = DESTROY;
				}
			}
			flushAllSprites();
		}
		//After the game loop clean the objectList, and flush loaded Sprites as to conserve memory.

		flushObjectList(env);
		env->player =0;
		env->gameState->points += env->points;
		env->points =0;
		flushAllSprites();
		displayClear();

		displayFinished(env);

		//ToDo: integrate shopScreen
		/**
		if(env->level % 2 ==0){
			shopScreen();
		}
		**/

    }
}



//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	timerInit();  // "Systemzeit" initialisieren
	buttonsInit();
	displayInit();
	spriteInit();

	//Timer0 Initialization
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS00)|(1<<CS02);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 255;

}


