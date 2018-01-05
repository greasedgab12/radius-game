#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"


#include "display.h"
#include "object.h"
#include "block.h"
#include "defines.h"


volatile Object objectList[MAXOBJECTS];
volatile Block blockList[MAXBLOCKS];


//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
volatile uint8_t inputBuffer;
volatile Environment env;

void init();



void initEnvironment(Object* objectList, Block* blockList){
    env->buttons =0;
    env->time = getMsTimer();
    env->gameState =0;
    env->blockList = blockList;
    env->bPos = 0;
    env->objectList = objectList;
    env->oPos = 0;
}

void updateEnvironment(){
	cli();
	env->buttons = inputBuffer;
	sei();
	env->time = getMsTimer();
}





SIGNAL (TIMER0_COMPA_vect){
	inputBuffer  = (B_UP<<7)| (B_DOWN<<6)| (B_LEFT<<5)| (B_RIGHT<<4);
	inputBuffer |= (B_A<<3)| (B_B<<2)| (B_PAUSE<<1)| (B_SELECT<<0);
}

int main(void)
{

	init();
	while(1){
		updateEnvironment();
		uint8_t i;
		for(i=0; i<env->oPos; i++){
			env->objectList[i]->think(objectList[i], env);
		}
		Block* overlaps = checkBlockCollision(env->blockList,env->bPos);
		for(i=0; i<env->bPos; i++){
			drawBlock(env->blockList[i]);
		}
		i=0;
		while(overlaps[i]!=0){
			drawBlock(overlaps[i]);
			free(&overlaps[i]);
			i++;
		}
		free(&overlaps[i]);
		free(overlaps);

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
	initWindow();
	//Timer0 Initialization
    TCCR0A |= (1<<WGM01);
    TCCR0B |= (1<<CS00)|(1<<CS02);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 255;
    //Environment Initialization
    Object objectList[MAXOBJECTS];
    Block blockList[MAXBLOCKS];
    initEnvironment(objectList, blockList);
}


