#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <inttypes.h>
#include <avr/eeprom.h>

GameState EEMEM EEsave1;
GameState EEMEM EEsave2;
GameState EEMEM EEsave3;
uint8_t   EEMEM EEcurrent_safe = 1 ;


#endif
