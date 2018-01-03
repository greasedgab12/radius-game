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

struct Environment{
	uint8_t buttons;
	uint32_t time;
	uint8_t gameState;
} env;

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
	uint8_t data[] = {28,196, //dino.bmp 	 size: 28x28
			0xff,0xff,0xff,0xff,0xff,0xff,0x3f,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0xc,0xc,0xc,0x30,
			0x3,0x0,0x0,0xc,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc,0xc,0x30,0x3,0x0,0x0,0x0,0xc0,0xc,0x30,0x3,0x0,0xf0,0xff,0xc0,0xc,0x30,
			0x3,0x0,0x0,0xff,0xc3,0xc,0x30,0x3,0x0,0x0,0xfc,0xf,0x0,0x30,0x3,0x0,0x0,0xf0,0x3f,0x0,0x30,0x3,0x0,0x0,0xf0,0xff,0x0,0x30,
			0x3,0x0,0x0,0xfc,0xff,0xff,0x30,0x3,0x0,0x0,0xff,0xff,0xcf,0x30,0x3,0x0,0xc0,0xff,0xff,0x3,0x30,0x3,0x0,0xf0,0xff,0xff,0x0,0x30,
			0x3,0xff,0xff,0xff,0xff,0x3,0x30,0xc3,0xff,0xff,0xff,0xff,0xff,0x30,0xc3,0xfc,0xff,0xff,0x3f,0xc0,0x30,0xc3,0xff,0xff,0xff,0xf,0x0,0x30,
			0xc3,0xff,0xff,0xff,0x0,0x0,0x30,0xc3,0xff,0xf,0x3,0x0,0x0,0x30,0xc3,0xff,0xc,0xf,0x0,0x0,0x30,0xc3,0xff,0xc,0x0,0x0,0x0,0x30,
			0x3,0xff,0x0,0x0,0x0,0x0,0x30,0x33,0x0,0x0,0x0,0x0,0x0,0x33,0x3,0x0,0x0,0x0,0x0,0x0,0x30,0xff,0xff,0xff,0xff,0xff,0xff,0x3f
			};
	init();
	initWindow();
	Sprite test = newSprite(4,4,data);
	drawSprite(mapSprite(newSprite(5,7,data)));
	uint8_t i = 0;
	uint16_t timer1 = 0;

	while (1) {

		uint16_t timer_main = getMsTimer();


		if(B_SELECT && timer1 +45<= timer_main){
			i++;
			moveSprite(test,i, test->y);
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


