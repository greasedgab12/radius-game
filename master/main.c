#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"

#include "display.h"
#include "environment.h"
#include "object.h"
#include "block.h"

#define MAXOBJECTS 32
#define MAXBLOCKS 32
#define MAXOVERLAPS 16

volatile Object objectList[MAXOBJECTS];
volatile uint8_t oPos=0;
volatile Block blockList[MAXBLOCKS];
volatile uint8_t bPos=0;
volatile Block overlapList[MAXOVERLAPS];
volatile uint8_t ovPos=0;


//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
volatile uint8_t inputBuffer;
volatile Environment env;

void init();

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
    env->buttons =0;
    env->time = getMsTimer();
    env->gameState =0;
}


