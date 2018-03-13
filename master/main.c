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
	**/

	//ToDo
	//env->gameState = main_menu(env);

	//ToDo: Couple gameState with selection in MainMenu.
	//env->gameState = newGame();

    Object enemy;

    //Game loop
    while(1){
    	//Reloading the player object.
		env->player = playerFromGameState(env, env->gameState);
		env->player->setXY(env->player,0, (MAXY - MINY)/2 -env->player->ly/2);
		addObject(env, env->player);

		env->enemyRemaining = 10;
		env->enemyMax =1;

		//ToDo: Add displayLevel
    	//ToDo: Add displayStart
		//Prevent the games entities from thinking too much.


		updateEnvironment(env);


		while(env->enemyRemaining || env->enemyCount){
			//put the game pause menu here
			if(env->buttons & M_P)
			{
				if(pause_menu(env)){
					print("pause 1",1,1);
				}
				else
				{
					print("pause 0",1,1);
				}
				updateEnvironment(env);
				updateEnvironment(env);
			}

			//Update Environment variables.
			updateEnvironment(env);
			//ToDo: Add HUD
			drawHud(env);
			printN(env->points,100,0);


			if(env->enemyCount < env->enemyMax && env->enemyRemaining){
				//ToDo Implement getNextEnemy(Environment mainEnv, Object enemy)
				enemy = newEnemyGlider(MINY +random()%(MAXY-MINY),(uint8_t)random()/2);
				addObject(env,enemy);
				env->enemyCount++;
				env->enemyRemaining--;
			}

			//For each passed frame execute think of each object.

			for(i=0; i<env->time-env->lastTime+1; i++){
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
					sendWindow(env->objectList[i]->x,env->objectList[i]->py,
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
		}
		//After the game loop clean the objectList, and flush loaded Sprites as to conserve memory.

		flushObjectList(env);
		env->player =0;
		flushAllSprites();
		displayClear();

		//ToDo: integrate shopScreen

		if(env->level % 2 ==0){
			env->gameState = shop_menu(env);
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

	//Timer0 Initialization
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS00)|(1<<CS02);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 255;

}


