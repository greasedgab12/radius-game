#include "buttons.h"
#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "savegame.h"


uint8_t set_options = 0;

void display_mainmenu(void)
{
	saveGame(10);
	uint8_t menu_state = MEN_MAIN;
	uint8_t menu_cursor = 1;

	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;

	print("/",13,1 + 4 * menu_cursor);

	while(1){
		time = getMsTimer();

		if( B_UP && ( time >= time_old))//move cursor up
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor-=1;

			//upper cursor boundaries
			if(menu_state == MEN_SAVEGAME && menu_cursor <=2 )menu_cursor = 2;
			else if(menu_state == MEN_NEWGAME && menu_cursor <=2 )menu_cursor = 2;
			else if(menu_state == MEN_HIGHSCORES && menu_cursor <=2 )menu_cursor = 2;
			else if(menu_state == MEN_OPT3 && menu_cursor <=3 )menu_cursor = 3;
			else if(menu_cursor <= 1)menu_cursor = 1;

			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
			time_old = getMsTimer() + MENU_DELAY;
		}


		if(B_DOWN && ( time >= time_old))	//move cursor down
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor+=1;

			//lower cursor boundaries
			if(menu_state==MEN_MAIN){				if(menu_cursor >= SIZE_MEN_MAIN)		menu_cursor = SIZE_MEN_MAIN;}
			else if(menu_state==MEN_PLAY){			if(menu_cursor >= SIZE_MEN_PLAY)		menu_cursor = SIZE_MEN_PLAY;}
			else if(menu_state==MEN_HIGHSCORES){	if(menu_cursor >= SIZE_MEN_HIGHSCORES)	menu_cursor = SIZE_MEN_HIGHSCORES;}
			else if(menu_state==MEN_OPTIONS){		if(menu_cursor >= SIZE_MEN_OPTIONS)		menu_cursor = SIZE_MEN_OPTIONS;}
			else if(menu_state==MEN_MUSIC){			if(menu_cursor >= SIZE_MEN_MUSIC)		menu_cursor = SIZE_MEN_MUSIC;}

			else if(menu_state==MEN_SAVEGAME){		if(menu_cursor >= SIZE_MEN_SAVEGAME)	menu_cursor = SIZE_MEN_SAVEGAME;}
			else if(menu_state==MEN_NEWGAME){		if(menu_cursor >= SIZE_MEN_NEWGAME)		menu_cursor = SIZE_MEN_NEWGAME;}
			else if(menu_state==MEN_OPT3){			if(menu_cursor >= SIZE_MEN_OPT3)		menu_cursor = SIZE_MEN_OPT3;}

			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);

			time_old = getMsTimer() + MENU_DELAY;
		}


		if(B_A && ( time >= time_old))
		{
			if(menu_state == MEN_MAIN)
			{
				menu_state = menu_cursor;
				menu_cursor = 1;
			}

			else if(menu_state == MEN_PLAY)
			{
				menu_state = menu_cursor + 10;
				menu_cursor = 2;
			}

			else if(menu_state == MEN_SAVEGAME)		menu_state = MEN_SAVEGAME_FUNC;

			else if(menu_state == MEN_NEWGAME)		menu_state = MEN_NEWGAME_FUNC;

			else if(menu_state == MEN_HIGHSCORES)	menu_cursor = 2;

			else if(menu_state == MEN_OPTIONS)		menu_state = menu_cursor + 30;

			else if(menu_state == MEN_OPT3)			menu_state = MEN_OPT3_DEL;

			else if(menu_state == MEN_MUSIC)		uart_puti(menu_cursor);	//send tracknumber 1->5 to soundController should lock smth like this

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();

			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}


		if(B_B && ( time >= time_old))	//go back to origin menu
		{
			if((menu_state == MEN_SAVEGAME) || (menu_state == MEN_NEWGAME))menu_state = MEN_PLAY;
			else if(menu_state == MEN_OPT3)menu_state = MEN_OPTIONS;
			else if(menu_state != MEN_MAIN)menu_state = MEN_MAIN;

			menu_cursor = menu_state % 10;

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();
			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}

		switch(menu_state)
		{
		case MEN_PLAY:
			print("CONTINUE",20,5);
			print("Select Savegame",20,9);
			print("Start NEW Savegame",20,13);
			break;

		case MEN_CONTINUE:

			loadSavegame(getCurrentSave());
			displayClear();
			return;
			break;

		case MEN_SAVEGAME:
			print("SELECT TO CONTINUE",20,5);
			print("Save 1C",20,9);
			print("Save 2",20,13);
			print("Save 3",20,17);
			print("Save 4",20,21);
			break;

		case MEN_SAVEGAME_FUNC:
			loadSavegame(menu_cursor-1);
			displayClear();
			//print("con",0,6);
			//printN(menu_cursor-1,20,6);
			return;

		case MEN_NEWGAME:
			print("SELECT TO RESET",20,5);
			print("Save 1N",20,9);
			print("Save 2",20,13);
			print("Save 3",20,17);
			print("Save 4",20,21);
			break;

		case MEN_NEWGAME_FUNC:
			deleteSavegame(menu_cursor-1);
			loadSavegame(menu_cursor-1);
			displayClear();
			//print("New",0,0);
			//printN(menu_cursor-1,20,0);
			return;

		case MEN_HIGHSCORES:
			print("HIGHSCORES",20,5);
			print("Save 1H",20,9);	//not sorted only demo content
			print("Save 2",20,13);
			print("Save 3",20,17);
			print("Save 4",20,21);
			break;

		case MEN_OPTIONS:
			print("Music on off",20,5);
			print("Invert Colours",20,9);
			print("Delete all Saves",20,13);
			break;

		case MEN_OPT1:
			//not finished yet
			break;

		case MEN_OPT2:
			if(set_options)set_options = 0;
			else set_options = 1;
			displayInverse(set_options);
			menu_state = MEN_OPTIONS;
			menu_cursor = 2;
			break;

		case MEN_OPT3:
			print("ALL PROGRESS WILL",20,5);
			print("BE LOST. CONTINUE F",20,9);
			print("NO",20,13);
			print("YES",20,17);
			break;

		case MEN_OPT3_DEL:
			deleteSavegame(1);
			deleteSavegame(2);
			deleteSavegame(3);
			deleteSavegame(4);
			menu_state = MEN_OPTIONS;
			break;

		case MEN_MUSIC:
			print("Track",20,5);
			print("Track",20,9);
			print("Track",20,13);
			print("Track",20,17);
			print("Track",20,21);
			break;

		case MEN_MAIN:
			print("PLAY ",20,5);
			print("HIGHSCORES",20,9);
			print("OPTIONS",20,13);
			print("PLAY MUSIC",20,17);
			break;

		case MEN_EXIT:
			displayClear();
			return;

		default:
			menu_state = 0;
		}
	}
}

void display_gamemenu(void)//not finished
{
	return;
}

