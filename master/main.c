#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <entities/general.h>
#include <entities/projectile.h>
#include <entity.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
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


#include "sprite.h"
#include "menu.h"


volatile Object objectList[MAXOBJECTS];


//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
volatile uint8_t inputBuffer;


void init();


void updateEnvironment(Environment env){
	cli();
	env->buttons = inputBuffer;
	sei();
	env->lastTime = env->time;
	//Cap time at approximately 60fps
	env->time = getMsTimer()/17;
}


SIGNAL (TIMER0_COMPA_vect){
	inputBuffer =0;
	if(B_UP){inputBuffer|=M_U;}
	if(B_DOWN){inputBuffer|=M_D;}
	if(B_LEFT){inputBuffer|=M_L;}
	if(B_RIGHT){inputBuffer|=M_R;}
	if(B_A){inputBuffer|=M_A;}
	if(B_B){inputBuffer|=M_B;}
	if(B_PAUSE){inputBuffer|=M_P;}
	if(B_SELECT){inputBuffer|=M_S;}
}

int main(void)
{
	init();
    uint8_t i,j;
    uint8_t t=0;
	//Environment Initialization

    Object objectList[MAXOBJECTS];
    for(i=0; i<MAXOBJECTS; i++){
    	objectList[i]=0;
    }
    //Environment is persistend throughout the entire runtime.
    Environment env = newEnvironment(objectList);

    //ToDo: Display Startmenu
	display_mainmenu();
	//ToDo: Couple gameState with selection in MainMenu.
	env->gameState = newGame();

	sei();
	//Prevent the games entities from thinking too much.
	env->lastTime = getMsTimer()/17;

	while(1){

		//Update Environment variables.
		updateEnvironment(env);


		if(env->enemiesToSpawn){
			//ToDo Implement getNextEnemy(Environment mainEnv, Object enemy)
			if(enemy){
				addObject(env,enemy);
			}
			enemy = newEnemyGlider((uint8_t)random()/16*4,(uint8_t)random()/2);
			addObject(env,enemy);
			t++;
		}

		//For each passed frame execute think of each object.
		for(i=0; i<env->time-env->lastTime+1; i++){
			for(j=0; j<env->oPos; j++){
				env->objectList[j]->think(env->objectList[j], env);
			}
		}


		//Clean up dead objects and draw alive ones if they haven't been drawn already.
		for(i=0; i<env->oPos; i++){
			if(env->objectList[i]->isAlive){
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
			if(env->objectList[i]->isAlive==0){
				if(env->objectList[i]->type == ENEMY){
					t--;
				}
				removeObject(env, objectList[i]);

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

}



//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	timerInit();  // "Systemzeit" initialisieren
	buttonsInit();
	displayInit();

	//Timer0 Initialization
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS00)|(1<<CS02);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 255;

}


