#include "savegame.h"
#include <inttypes.h>
#include <avr/eeprom.h>


void deleteSavegame(uint8_t saveNumber)//not finished
{
	//write 0 to savegame saveNumber location in eeprom
	//eeprom_write_byte ((uint8_t*) location of savenumber, 0);
	return;
}

void loadSavegame(uint8_t saveNumber)//not finished
{
	//read data in location of saveNumber
	//data = eeprom_read_byte((uint8_t*)23);
	return;
}

void saveGame(uint8_t saveNumber)//not finished
{
	//write all data to eeprom location saveNumber
	//eeprom_write_byte ((uint8_t*) CURRENT_SAVEGAME, saveNumber);
	eeprom_write_byte ((uint8_t*) CURRENT_SAVEGAME, saveNumber);
}

uint8_t getCurrentSave()//not finished
{
	//return last play saveNumber
	return eeprom_read_byte((uint8_t*)CURRENT_SAVEGAME); // read the byte in location 23
}
