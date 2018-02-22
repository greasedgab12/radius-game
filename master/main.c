#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"

#include "char.h"
#include "display.h"
#include "object.h"
#include "block.h"
#include "defines.h"

volatile const uint8_t dino[] = {28,196,      //dino.bmp
0xff,0xff,0xff,0xff,0xff,0xff,0x3f,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0xc,0xc,0xc,0x30,
0x3,0x0,0x0,0xc,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc0,0xc,0x30,0x3,0x0,0xf0,0xff,0xc0,0xc,0x30,
0x3,0x0,0x0,0xff,0xc3,0xc,0x30,0x3,0x0,0x0,0xfc,0xf,0x0,0x30,0x3,0x0,0x0,0xf0,0x3f,0x0,0x30,0x3,0x0,0x0,0xf0,0xff,0x0,0x30,
0x3,0x0,0x0,0xfc,0xff,0xff,0x30,0x3,0x0,0x0,0xff,0xff,0xcf,0x30,0x3,0x0,0xc0,0xff,0xff,0x3,0x30,0x3,0x0,0xf0,0xff,0xff,0x0,0x30,
0x3,0xff,0xff,0xff,0xff,0x3,0x30,0xc3,0xff,0xff,0xff,0xff,0xff,0x30,0xc3,0xfc,0xff,0xff,0x3f,0xc0,0x30,0xc3,0xff,0xff,0xff,0xf,0x0,0x30,
0xc3,0xff,0xff,0xff,0x0,0x0,0x30,0xc3,0xff,0xf,0x3,0x0,0x0,0x30,0xc3,0xff,0xc,0xf,0x0,0x0,0x30,0xc3,0xff,0xc,0x0,0x0,0x0,0x30,
0x3,0xff,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0xff,0xff,0xff,0xff,0xff,0xff,0x3f
};


volatile Object objectList[MAXOBJECTS];
volatile Block blockList[MAXBLOCKS];


//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
volatile uint8_t inputBuffer;


void moveOnButton(Object self, Environment mainEnv){

	char test[9];
	itoa(mainEnv->buttons, test, 10);
	print(test, 80, 8);
	if(mainEnv->buttons & M_U){
		self->move(self,mainEnv, self->x, self->y-1);
	}
	else if(mainEnv->buttons & M_D){
		self->move(self,mainEnv, self->x, self->y+1);
	}
	else if(mainEnv->buttons & M_L){
		self->move(self,mainEnv, self->x-1, self->y);
	}
	else if(mainEnv->buttons & M_R){
		self->move(self, mainEnv, self->x+1, self->y);
	}

}

uint8_t nop(){return 0;}

void init();


void updateEnvironment(Environment env){
	cli();
	env->buttons = inputBuffer;
	sei();

	env->time2 = getMsTimer();
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
    //Environment Initialization
    Object objectList[MAXOBJECTS];
    Block blockList[MAXBLOCKS];
	Environment env = newEnvironment(objectList, blockList);
	char test[9];
	Object obj1 = newObject(40,40,dino);
	obj1->think = &moveOnButton;
	obj1->collide = &nop;
	env->objectList[0]=obj1;
	env->oPos++;
	sei();
	obj1->representation=mapObject(obj1);

	while(1){

		updateEnvironment(env);

	/**
		if(env->buttons & M_U){
			obj1->move(obj1,env, obj1->x, obj1->y+1);
		}
		else if(env->buttons & M_D){
			obj1->move(obj1,env, obj1->x, obj1->y-1);
		}
		else if(env->buttons & M_L){
			obj1->move(obj1,env, obj1->x+1, obj1->y);
		}
		else if(env->buttons & M_R){
			obj1->move(obj1, env, obj1->x-1, obj1->y);
		}
	**/

		uint8_t i;

		for(i=0; i<env->oPos; i++){
			itoa(env->buttons, test, 10);
			print(test, 80, 6);
			env->objectList[i]->think(env->objectList[i], env);
			if(env->objectList[i]->representation==0){
				env->objectList[i]->representation = mapObject(env->objectList[i]);
			}
			if(env->objectList[i]->representation->blockType == NOTDRAWN){
				drawBlock(env->objectList[i]->representation);
			}
		}

		//Block* overlaps = checkBlockCollision(env->blockList,env->bPos);
		/**
		for(i=0; i<env->bPos; i++){
			while(env->blockList[i]->blockType == DESTROY){
				releaseBlock(env->blockList[i]);
				for(j=i; j<env->bPos-1; j++){
					env->blockList[j] = env->blockList[j+1];
				}
				env->bPos= env->bPos>0?env->bPos-1:0;
			}
		}

		i=0;
		while(overlaps[i]!=0){
			drawBlock(overlaps[i]);
			i++;
		}
		free(overlaps);
		**/
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

}


