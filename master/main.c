#include <stdlib.h>
#include <avr/io.h>

#include <avr/eeprom.h>

#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>


#include "uart.h"
#include "timer.h"

#include "defines.h"
#include "structure.h"
#include "environment.h"

#include "char.h"
#include "display.h"

#include "savegame.h"
#include "game.h"


#include "menu.h"
#include "sprite.h"




void init();




int main(void)
{
	init();
	titleScreen();

	uint8_t i,j=0;



	//Environment Initialization
    //Environment is persistend throughout the entire runtime.
    Environment env;
    sei();

    while(1){
    	env = newEnvironment();
		flushObjectList(env);
		flushAllSprites();
    	_delay_ms(200);
		main_menu(env);
		uint8_t continueGame = 1;


		//Game loop
		while(continueGame){
			//Reloading the player object.
			getPlayerFromGameState(env);
			env->player->setXY(env->player,0, (MAXY - MINY)/2 -env->player->ly/2);

			env->level = env->gameState->level;

			env->enemyRemaining =  4 ;//env->level;
			env->enemyMax =1 + (env->level/2<5?env->level/2:5);

			displayClear();
			displayLevel(env);
			displayStart(env);

			//Prevent the games entities from thinking too much.
			updateEnvironment(env);
			//Force redraw of HUD
			drawHud(1,2,1,2,1);

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


				drawHud(env->player->entity->health,env->player->entity->maxHealth,env->player->entity->energy,env->player->entity->maxEnergy, env->points);


				//Enemy spawn check.
				if(env->enemyCount < env->enemyMax && env->enemyRemaining){
					if(isSpawnListEmpty(env)){
						//Only set the spawn list if it is empty.
						setSpawnList(env);
					}
					else{
						//If there are entries left, check wether they are to be activated.
						getNextEnemy(env);
					}
				}

				//For each passed frame since last call, execute think of each object.
				for(i=0; i<env->time-(env->lastTime); i++){
					for(j=0; j<MAXOBJECTS; j++){
						if(env->objectList[j]->activeState == ACTIVE){
							env->objectList[j]->think(env->objectList[j], env);
						}
					}
				}

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
				if(env->time -env->lastTime < 3){
					checkMappedSpriteCollision(env->objectList);
				}
				//Update drawState of every object.
				for(i=0; i<MAXOBJECTS; i++){
					if(env->objectList[i]->activeState == ACTIVE){
						if(env->objectList[i]->drawState== NOTDRAWN){
							env->objectList[i]->drawState = DRAWN;
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
				//Apply points to the GameState.
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
	displayInit();
	spriteInit();

	//Timer0 Initialization
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS00)|(1<<CS02);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 255;
    //Button Initialization
    DDRD &=~( (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7));
	PORTD |=( (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7));

	DDRC &= ~( (1<<4) | (1<<5));
	PORTC |= ( (1<<4) | (1<<5));

}


