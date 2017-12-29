#include <util/delay.h>
#include <avr/io.h>
#include <inttypes.h>
#include "display.h"

//set registers for window option 2 
#define WINDOW_SETOPTIONS_RAM	0b10001011		//ram access control (cmd13)
#define WINDOW_SETOPTIONS_LCD	0b11000110		//lcd mapping control (cmd19)

#define WINDOW_ENABLE			0b11111001		//(cmd 34)
#define WINDOW_DISABLE			0b11111000 		//(cmd 34)

#define WINDOW_STARTCOL			0b11110100 		//dualcommand 2nd is coordinates (cmd30)
#define WINDOW_STARTPAGE		0b11110101		//dualcommand 2nd is coordinates (cmd31)
#define WINDOW_ENDCOL			0b11110110 		//dualcommand 2nd is coordinates (cmd32)
#define WINDOW_ENDPAGE			0b11110111		//dualcommand 2nd is coordinates (cmd33)


void initWindow()	//call before use of sendWindwo
{
	sendbyte(WINDOW_SETOPTIONS_RAM,0);
	sendbyte(WINDOW_SETOPTIONS_LCD,0);
	sendbyte(WINDOW_DISABLE,0);
}

void sendWindow(uint8_t x,uint8_t y,uint8_t lx, uint8_t ly, uint8_t *data)
{
	sendbyte(WINDOW_STARTCOL,0);
	sendbyte(x,0);
	sendbyte(WINDOW_STARTPAGE,0);
	sendbyte(y,0);

	sendbyte(WINDOW_ENDCOL,0);
	sendbyte(x+lx,0);
	sendbyte(WINDOW_ENDPAGE,0);
	sendbyte(y+ly,0);

	sendbyte(WINDOW_ENABLE,0);

	uint8_t counter;
	if(data){
		for(counter = 2; counter < data[1]+2;counter++)
		{
			sendbyte(data[counter],1);
		}
	}
	else{
		for(counter =0; counter < lx*ly/4 +1; counter++){
			sendbyte(0,1);
		}
	}
	sendbyte(WINDOW_DISABLE,0);

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

void page(uint8_t x,uint8_t y,uint8_t h)//alle Pixel einer Page beschreiben
{
	sendbyte(15&x,0);//Adressentranslation
	sendbyte(16+(x>>4),0);
	sendbyte(96+y,0);
	sendbyte(h,1);
}

void clear(void)
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

void displayInit(void)
{
	DDRB |= (1<<5);//SCK
	DDRB |= (1<<3);//SDA
	DDRB |= (1<<4);//RST
	DDRB |= (1<<1);//CD
	DDRB |= (1<<0);//CS0
	PORTB |= (1<<4);//kein RST
	PORTB &= ~(1<<1);//CD auf low
	PORTB &= ~(1<<5);//SCK auf low
	PORTB |= (1<<0);_delay_us(10);PORTB &= ~(1<<0);//CS0 auf low
	sendbyte(241,0);//cmd(27)
	sendbyte(103,0);//cmd(27)
	sendbyte(198,0);//LCD mapping Hierum cmd(19)
	//~ sendbyte(192,0);//LCD mapping Hierum
	sendbyte(64,0);
	sendbyte(80,0);
	sendbyte(43,0);
	sendbyte(235,0);
	sendbyte(129,0);//vbias contrast
	sendbyte(95,0);
	sendbyte(137,0); //------
	uint16_t i;
	for(i=0;i<4160;i++)//RAM-Ausspühlen, alles weiß
	{
		sendbyte(0,1);
	}
	//~ DDRC |= 1;//Debug-LED
	sendbyte(0,0);//RAM-Zeigerpostition zurück auf 0/0
	sendbyte(16,0);
	sendbyte(96,0);
	sendbyte(175,0);//Bildschirm an
	//~ PORTC |= 1;//Debug-LED an==Init zu Ende
}
