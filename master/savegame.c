#include "buttons.h"
#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "game.h"
#include "savegame.h"
#include "environment.h"
#include "object.h"
#include "entities/stats.h"
#include <avr/eeprom.h>
#include "savegame.h"
#include "char.h"

struct GameState_Struct EEsave1;
uint32_t EEMEM EEhighscore;
/**
void prepEEPROM()
{
	uint32_t highscore = 0;
	//set all savegames to basic values
	eeprom_write_block(newGame(),  &EEsave1, sizeof(GameState));
	eeprom_write_block (&EEhighscore, &highscore,sizeof(uint32_t));

}
**/

uint32_t loadHighScore(){
	return eeprom_read_dword(&EEhighscore);
}

uint8_t saveHighScore(uint32_t highScore){
	uint32_t oldScore;
	oldScore = eeprom_read_dword(&EEhighscore);
	if(oldScore < highScore)
	{
		eeprom_write_dword (&EEhighscore, highScore);
		return 1;
	}
	return 0;
}

void loadSave(GameState safegame)
{
	eeprom_read_block(safegame, &EEsave1, sizeof(EEsave1));
}


void safeSave(GameState gamestate)
{
	uint32_t highScore = loadHighScore();
	if(gamestate->points > highScore)
	{
		eeprom_write_dword(&EEhighscore, gamestate->points);
	}
	eeprom_write_block(gamestate,  &EEsave1, sizeof(EEsave1));
}

