#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <inttypes.h>
#include <avr/eeprom.h>

extern GameState EEMEM EEsave1;
extern GameState EEMEM EEsave2;
extern GameState EEMEM EEsave3;
extern uint8_t   EEMEM EEcurrent_safe;


#endif
