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
	Environment env = newEnvironment(objectList);

	display_mainmenu();
	
	Object obj1 = newPlayer(20,20);
	obj1->entity->weaponA = newLauncher(0);
	printN(obj1->entity->weaponA,0,2);
	obj1->entity->weaponB = newHeavy(0);

	printN(obj1->entity->weaponB,0,4);
	addObject(env,obj1);

	Object enemy = 0;
	sei();
	env->lastTime = getMsTimer()/17;
	while(1){
		updateEnvironment(env);

		//For each passed frame execute think of each object.

		if((t<5) && env->time > env->lastTime){
			enemy = newEnemyGlider((uint8_t)random()/16*4,(uint8_t)random()/2);
			addObject(env,enemy);
			t++;
		}


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
		if(env->lastTime -env->time == 1){
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


