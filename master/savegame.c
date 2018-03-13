#include "buttons.h"
#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "savegame.h"
#include "environment.h"
#include "object.h"
#include "entities/stats.h"
#include <avr/eeprom.h>
#include "savegame.h"

uint8_t getCurrentSave()//not finished
{
	return eeprom_read_byte(&EEcurrent_safe);
}

GameState loadSave(uint8_t safe_number)
{
	GameState safegame = newGame();
	if(safe_number ==1)
	{
		eeprom_read_block(&safegame, &EEsave1, sizeof(EEsave1));
	}
	else if(safe_number ==2)
	{
		eeprom_read_block(&safegame, &EEsave2, sizeof(EEsave2));
	}
	else if(safe_number ==3)
	{
		eeprom_read_block(&safegame, &EEsave3, sizeof(EEsave3));
	}
	else
	{
		print("E read error",1,1);
	}
	return safegame;
}


void safeSave(uint8_t safe_number, GameState gamestate)
{
	if(safe_number == 1)
	{
		eeprom_write_block(&gamestate,  &EEsave1, sizeof(EEsave1));
		eeprom_write_byte(&EEcurrent_safe,safe_number);
	}
	else if(safe_number == 2)
	{
		eeprom_write_block(&gamestate,  &EEsave2, sizeof(EEsave2));
		eeprom_write_byte(&EEcurrent_safe,safe_number);
	}
	else if(safe_number == 3)
	{
		eeprom_write_block(&gamestate,  &EEsave3, sizeof(EEsave3));
		eeprom_write_byte(&EEcurrent_safe,safe_number);
	}
	else
	{
		print("E write error",1,1);
	}
	return;
}
