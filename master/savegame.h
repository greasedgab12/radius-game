#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <inttypes.h>
#include <avr/eeprom.h>

extern GameState EEMEM EEsave1;
extern uint32_t EEMEM EEhighscore;


void checkHighScore(uint32_t score);
GameState loadSave();
void safeSave(GameState gamestate);



#endif
