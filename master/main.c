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
#include "menu.h"


#include "sprite.h"



void init();




void titleScreen(){

	drawTitleScreen();
	_delay_ms(1000);
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

void displayGameOver(Environment env){
	print("Game Over", 57, 5);
	_delay_ms(300);
	if(saveHighScore(env->gameState->points)){
		print("NEW HIGHSCORE:",9,9);
		_delay_ms(300);
		printN(env->gameState->points,18,13);
	}
	_delay_ms(2000);
}


int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


int main(void)
{
	init();

	//titleScreen();

	uint8_t i,j=0;



	//Environment Initialization
    //Environment is persistend throughout the entire runtime.
    Environment env = newEnvironment();
    sei();

    while(1){
		flushObjectList(env);
		flushAllSprites();
    	_delay_ms(200);
		main_menu(env);
		uint8_t continueGame = 1;
		//ToDo: Couple gameState with selection in MainMenu.


		//Game loop
		while(continueGame){
			//Reloading the player object.
			getPlayerFromGameState(env);
			env->player->setXY(env->player,0, (MAXY - MINY)/2 -env->player->ly/2);

			env->level = env->gameState->level;

			env->enemyRemaining =  4 + 2*env->level;
			env->enemyMax =1 + (env->level/2<5?env->level/2:5);

			displayClear();
			displayLevel(env);
			displayStart(env);
			//Prevent the games entities from thinking too much.



			updateEnvironment(env);
			//Force redraw of HUD
			drawHud(1,2,1,2,0);

			while(env->enemyRemaining || env->enemyCount){
				//Update Environment variables.
				updateEnvironment(env);
				if(env->buttons & M_P){
					continueGame = pause_menu(env);
					if(!continueGame){
						break;
					}
					drawHud(1,2,1,2,0);
					for(i=0; i<MAXOBJECTS; i++){
						if(env->objectList[i]->activeState == ACTIVE){
							env->objectList[i]->drawState = NOTDRAWN;
						}
					}
				}
				if(env->player->killedBy != 0){
					env->gameState->points += env->points;
					displayGameOver(env);
					continueGame = 0;
					if(!continueGame){
						break;
					}

				}



				printN(freeRam(),0,2);
				printN(env->time,0,4);
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
					for(j=0; j<MAXOBJECTS; j++){
						if(env->objectList[j]->activeState == ACTIVE){
							env->objectList[j]->think(env->objectList[j], env);
						}
					}
				}

				//printN(env->time, 64,0);
				//Clean up dead objects and draw alive ones if they haven't been drawn already.
				for(i=0; i<MAXOBJECTS; i++){
					if(env->objectList[i]->killedBy==0){
						if((env->objectList[i]->activeState == ACTIVE) &&(env->objectList[i]->drawState == NOTDRAWN)){
							drawObject(env->objectList[i]);
						}
					}
				}

				//Remove dead objects from list.
				for(i=0; i<MAXOBJECTS; i++){
					if(env->objectList[i]->killedBy!=0){
						if(env->objectList[i]->activeState==ACTIVE){
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
							sendWindow(env->objectList[i]->x,env->objectList[i]->y/4,env->objectList[i]->lx,env->objectList[i]->msly,0);
							env->objectList[i]->activeState =EMPTY;
						}
					}
				}

				//Check, wether drawn sprites overlap one another and draw overlapping parts again.
				//Skip if frames are dropped.
				if(env->time -env->lastTime < 3){
					checkMappedSpriteCollision(env->objectList);
				}
				//Update drawState of every object.
				for(i=0; i<MAXOBJECTS; i++){
					if(env->objectList[i]->activeState == ACTIVE){
						if(env->objectList[i]->drawState== NOTDRAWN){
							env->objectList[i]->drawState = DRAWN;
						}
						if(env->objectList[i]->drawState == DRAWONCE){
							env->objectList[i]->drawState = DESTROY;
						}
					}
					else{
						env->objectList[i]->drawState = DRAWN;
					}
				}
			}
			//After the game loop clean the objectList, and flush loaded Sprites as to conserve memory.

			flushObjectList(env);
			flushAllSprites();
			if(continueGame){
				env->gameState->level++;
				env->gameState->points += env->points;
				env->points =0;

				displayClear();

				displayFinished(env);

				if(env->level % 2 ==0){
					shop_menu(env);
				}
			}
		}
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


