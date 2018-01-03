#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "buttons.h"

#include "display.h"
#include "sprite.h"

volatile Sprite spriteList[MAXSPRITES];
volatile Blank deleteList[MAXDELETES];
volatile Sprite objectList[MAXOBJECTS];
volatile uint8_t sPos=0;
volatile uint8_t dPos=0;
volatile uint8_t oPos=0;

//The Buttons are assigned to the inputBuffer in the following order (Most significant Bit first):
//UP, DOWN, LEFT, RIGHT, A, B, PAUSE, SELECT
volatile uint8_t inputBuffer;

struct Environment{
	uint8_t buttons;
	uint32_t time;
	uint8_t gameState;
} env;

void init();

void updateEnvironment(){
	cli();
	env->buttons = inputBuffer;
	sei();
	env->time = getMsTimer();
}

void delSprite(Sprite instance){
    //if our sprite happens to be a nullptr, return
    if(instance == 0 || sPos == 0 || dPos == (MAXDELETES -1)){
        return;
    }
    //add blank to our blank list.
    deleteList[dPos] = newBlank(instance);
    dPos++;
    //search for sprite in spriteList for removal.
    uint8_t i;
    for(i=0; i<=sPos; i++){
        if(spriteList[i] == instance){
            //remove sprite and move the following items up one place.
            uint8_t j;
            for(j=i; j<sPos; j++){
                spriteList[j] = spriteList[j+1];
            }
            spriteList[sPos] = 0;
            sPos--;
            return;
        }
    }
}

void drawSprite(Sprite instance){
	if(sPos < MAXSPRITES){
		spriteList[sPos] = instance;
    	sPos++;
	}
}
void moveSprite(Sprite instance, uint8_t nx, uint8_t ny){
    delSprite(instance);
    instance->setXY(instance, nx, ny);
    drawSprite(instance);
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


