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
#include "char.h"

GameState EEMEM EEsave1;
uint32_t EEMEM EEhighscore;

void prepEEPROM()
{
	uint32_t highscore = 0;
	//set all savegames to basic values
	eeprom_write_block(newGame(),  &EEsave1, sizeof(GameState));
	eeprom_write_block (&EEhighscore, &highscore,sizeof(uint32_t));
}


GameState loadSave()
{
	GameState safegame = newGame();
	eeprom_read_block(&safegame, &EEsave1, sizeof(EEsave1));
	return safegame;
}


void safeSave(GameState gamestate)
{
	uint32_t highscore;
	eeprom_read_block (&highscore,&EEhighscore, sizeof(uint32_t));
	if(gamestate->points > highscore)
	{
		eeprom_write_block (&EEhighscore, &highscore,sizeof(uint32_t));
	}
	eeprom_write_block(&gamestate,  &EEsave1, sizeof(EEsave1));
}

