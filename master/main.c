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

void init();

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



int main(void)
{
	//Initialisierung ausfuehren
	uint8_t data[] = {4,16,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	init();
	initWindow();
	Sprite test = newSprite(5,0,data);
	drawSprite(test);
	uint8_t i = 0;
	uint16_t timer1 = 0;

	while (1) {

		uint16_t timer_main = getMsTimer();


		if(B_SELECT && timer1 +45<= timer_main){
			i++;
			//moveSprite(test,i, test->y);
			timer1 = timer_main;
		}

		uint8_t iter =0;
		/*
		for(iter = dPos-1; iter>=0; iter--){
			Blank current = deleteList[iter];
			sendWindow(current->x, current->y, current->lx, current->ly,0);
			deleteList[iter] = 0;
			free(current);
		}
		*/
		dPos =0;
		for(iter = 0; iter<sPos; iter++){
			Sprite current = spriteList[iter];
			sendWindow(current->x, current->y, current->lx, current->ly,current->data);
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
}


