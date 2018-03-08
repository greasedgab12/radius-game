#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <entities/general.h>
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
#include "entities/bullet.h"
#include "entities/general.h"
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

	env->time = getMsTimer();
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
    uint8_t i;
	//Environment Initialization
    Object objectList[MAXOBJECTS];
    for(i=0; i<MAXOBJECTS; i++){
    	objectList[i]=0;
    }
	Environment env = newEnvironment(objectList);
	
	//display_mainmenu();
	
	Object obj1 = newPlayer(20,20);
	addObject(env,obj1);

	sei();

//	while(1)
//	{
//		if(B_A)uart_putc('a');
//		else if(B_B)uart_putc('b');
//		else if(B_UP)uart_putc('c');
//		else if(B_DOWN)uart_putc('d');
//		else if(B_LEFT)uart_putc('e');
//		else if(B_RIGHT)uart_putc('f');
//		else if(B_PAUSE)uart_putc('g');
//		else if(B_SELECT)uart_putc('h');
//
//	}


	while(1){
		updateEnvironment(env);
		if(env->buttons & M_P) obj1 = display_gamemenu(obj1);



		for(i=0; i<env->oPos; i++){
			env->objectList[i]->think(env->objectList[i], env);
		}

		for(i=0; i<env->oPos; i++){
			if(env->objectList[i]->drawState == NOTDRAWN){
				drawObject(env->objectList[i]);
			}
		}

		checkMappedSpriteCollision(env->objectList,env->oPos);

		for(i=0; i<env->oPos; i++){
			if(env->objectList[i]->drawState== NOTDRAWN){
				env->objectList[i]->drawState = DRAWN;
			}
			if(env->objectList[i]->drawState == DRAWONCE){
				env->objectList[i]->drawState = DESTROY;
			}
		}

		for(i=0; i<env->oPos; i++){
			printN(i, 16, 2+i*2);
			if(env->objectList[i]->isAlive==0){
				removeObject(env, objectList[i]);
			}
		}
		print("done",0,16);






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


