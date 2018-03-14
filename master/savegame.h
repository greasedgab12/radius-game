#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <inttypes.h>
#include <avr/eeprom.h>

extern struct GameState_Struct EEMEM EEsave1;
extern uint32_t EEMEM EEhighscore;


uint32_t loadHighScore();
uint8_t saveHighScore(uint32_t highScore);

void loadSave(GameState safegame);
void safeSave(GameState gamestate);



#endif
