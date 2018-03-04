#include <util/delay.h>
#include <avr/io.h>
#include <inttypes.h>
#include "display.h"
#include <avr/interrupt.h>
#include <char.h>
#include <inttypes.h>

void sendWindow(uint8_t x,uint8_t y,uint8_t lx, uint8_t ly, const uint8_t data[])
{
	sendbyte(WINDOW_STARTCOL,0);
	sendbyte(x,0);
	sendbyte(WINDOW_STARTPAGE,0);
	sendbyte(y,0);

	sendbyte(WINDOW_ENDCOL,0);
	sendbyte(x+lx,0);
	sendbyte(WINDOW_ENDPAGE,0);
	sendbyte(y+ly-1,0);

	sendbyte(WINDOW_ENABLE,0);

	uint8_t counter;
	if(data){
		for(counter = 2; counter < data[1]+2;counter++)
		{
			sendbyte(data[counter],1);
		}
	}
	else{
		for(counter =0; counter < lx*ly ; counter++){
			sendbyte(0,1);
			//  85 gray
		}
	}
	sendbyte(WINDOW_DISABLE,0);
}

void displayForceOn(uint8_t option)
{
	if(option >= 1)option = 1;
	sendbyte( DISPLAY_FORCE_ON | (0b00000001 & option)  ,0);
}

void displayInverse(uint8_t option)
{
	if(option >= 1)option = 1;
	sendbyte( DISPLAY_INVERSE | (0b00000001 & option)  ,0);
}


void displayClear()
{
	uint16_t i;
	sendbyte(0,0);//RAM-Zeigerpostition zurück auf 0/0
	sendbyte(16,0);
	sendbyte(96,0);
	for(i=0;i<4160;i++)//RAM-Ausspühlen, alles weiß
	{
		sendbyte(0,1);
	}
	sendbyte(0,0);//RAM-Zeigerpostition zurück auf 0/0
	sendbyte(16,0);
	sendbyte(96,0);

}

void sendbyte(uint8_t byte,uint8_t cd)
{

	int8_t i;
	if(cd)
		PORTB |= (1<<1);
	for(i=7;i>-1;i--)
	{
		PORTB &= ~(1<<5);
		if((1<<i)&byte)
			PORTB |= (1<<3);
		else
		{
			PORTB &= ~(1<<3);
		}
		PORTB |= (1<<5);
	}
	PORTB &= ~(1<<1);
}

void displayInitPorts(void)
{
	DDRB |= (1<<5);//SCK
	DDRB |= (1<<3);//SDA
	DDRB |= (1<<4);//RST
	DDRB |= (1<<1);//CD
	DDRB |= (1<<0);//CS0
	PORTB |= (1<<4);//kein RST
	PORTB &= ~(1<<1);//CD auf low
	PORTB &= ~(1<<5);//SCK auf low
	PORTB |= (1<<0);
	_delay_us(10);
	PORTB &= ~(1<<0);//CS0 auf low
}

void displayInit(void)
{
	displayInitPorts();

	sendbyte(SET_COM_END,0);
	sendbyte(SET_COM_VALUE,0);

	sendbyte(WINDOW_SETOPTIONS_LCD,0);

	sendbyte(SET_SCRL_LINE_LSB_0,0);
	sendbyte(SET_SCRL_LINE_MSB_0,0);
	sendbyte(SET_PANEL_LOADING,0);
	sendbyte(SET_DISPLAY_BIAS_RATIO,0);
	sendbyte(SET_VBIAS_POTI,0);
	sendbyte(VBIAS_VALUE,0);

	displayClear();

	sendbyte(WINDOW_SETOPTIONS_RAM,0);
	sendbyte(SET_COLADR_LSB_0,0);
	sendbyte(SET_COLADR_MSB_0,0);
	sendbyte(SET_PAGE_ADDRESS,0);

	sendbyte(WINDOW_DISABLE,0);
	sendbyte(DISPLAY_ENABLE,0);
}



