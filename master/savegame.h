#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <inttypes.h>

//defines for savegame functionality, not finished
#define CURRENT_SAVEGAME 2

void deleteSavegame(uint8_t saveNumber);
void loadSavegame(uint8_t saveNumber);
void saveGame(uint8_t saveNumber);
uint8_t getCurrentSave();



#endif
