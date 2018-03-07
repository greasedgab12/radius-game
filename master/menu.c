#include "buttons.h"
#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "savegame.h"
#include "object.h"
#include "entities/stats.h"

uint8_t set_options = 0b00000001;



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

			else if(menu_state == MEN_OPT3 && menu_cursor == 4)		menu_state = MEN_OPT3_DEL;
			else if(menu_state == MEN_OPT3 && menu_cursor == 3)		menu_state = MEN_OPTIONS;

			else if(menu_state == MEN_MUSIC)		menu_state = MEN_MUSIC_PLAY;

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();

			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}


		if(B_B && ( time >= time_old))	//go back to origin menu
		{
			if(menu_state){
				menu_cursor = menu_state % 10;
				if(menu_cursor <= 1)menu_cursor = 1;
			}

			if((menu_state == MEN_SAVEGAME) || (menu_state == MEN_NEWGAME))menu_state = MEN_PLAY;
			else if(menu_state == MEN_OPT3)menu_state = MEN_OPTIONS;
			else if(menu_state != MEN_MAIN)menu_state = MEN_MAIN;

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();
			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}

//		printN(menu_cursor,1,5);
//		printN(menu_state,1,7);
//		printB((set_options>>0),1,1);
//		printB((set_options>>1),1,3);

		switch(menu_state)
		{
		case MEN_PLAY:
			print("CONTINUE",20,5);
			print("Select Savegame",20,9);
			print("Start NEW Savegame",20,13);
			break;

		case MEN_CONTINUE:

			loadSavegame(getCurrentSave());

			menu_state = MEN_EXIT;
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
			menu_state = MEN_EXIT;
			break;

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

			menu_state = MEN_EXIT;
			break;

		case MEN_HIGHSCORES:
			print("HIGHSCORES",20,5);
			print("Save 1H",20,9);	//not sorted only demo content
			print("Save 2",20,13);
			print("Save 3",20,17);
			print("Save 4",20,21);
			break;

		case MEN_OPTIONS:
			if( (set_options>>0) & 1)
			{
				print("Music ON",20,5);
			}
			else
			{
				print("Music oFF",20,5);
			}

			if((set_options>>1) & 1)
			{
				print("Invert Colours ON  ",20,9);
			}
			else
			{
				print("Invert Colours OFF",20,9);
			}

			print("Delete all Saves",20,13);
			break;



		case MEN_OPT1:

			if( (set_options>>0) & 1)
			{
				set_options &= ~(1<<0);
			//print("Music off",20,5);
			}
			else
			{
				set_options |= 1<<0;
				//print("Music oN",20,5);
			}


			menu_state = MEN_OPTIONS;
			menu_cursor = 1;
			break;

		case MEN_OPT2:
			if((set_options>>1) & 1)
			{
				set_options &= ~(1<<1);
				//print("Invert Colours OFF  ",20,9);
			}
			else
			{
				set_options |= 1<<1;
				//print("Invert Colours ON",20,9);
			}
			displayInverse((set_options>>1));
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
			displayClear();
			menu_cursor = 3;
			print("/",13,1 + 4 * menu_cursor);
			menu_state = MEN_OPTIONS;
			break;

		case MEN_MUSIC:
			print("Track",20,5);
			print("Track",20,9);
			print("Track",20,13);
			print("Track",20,17);
			print("Track",20,21);
			break;

		case MEN_MUSIC_PLAY:
			uart_puti(menu_cursor);	//send tracknumber 1->5 to soundController should lock smth like this
			menu_state = MEN_MUSIC;
			print("PLAYING",56,1 + 4 * menu_cursor);
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


uint8_t weapons_bought = 1;
uint8_t ships_bought = 1;
uint16_t points = 1000;


Object display_gamemenu(Object Player)//not finished
{
	displayClear();
	uint8_t menu_state = GAME_MAIN;
	uint8_t menu_cursor = 1;


	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;
	uint8_t zeile = 9;
	print("/",13,1 + 4 * menu_cursor);


	while(1){
		time = getMsTimer();


		if( B_UP && ( time >= time_old))//move cursor up
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor-=1;

			if((menu_state == GAME_SHOP || menu_state == GAME_SHOP_SELECT|| menu_state == GAME_SHOP_WEAPONS|| menu_state == GAME_SHOP_SHIPS|| menu_state == GAME_SHOP_SELECT_WEAPON || menu_state == GAME_SHOP_SELECT_SHIP) && (menu_cursor <=2)) menu_cursor = 2;
			else if(menu_cursor <= 1) menu_cursor = 1;

			print("/",13,1 + 4 * menu_cursor);
			time_old = getMsTimer() + MENU_DELAY;

		}
		if( B_DOWN && ( time >= time_old))//move cursor up
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor+=1;

			//lower cursor boundaries
			if(menu_state==GAME_MAIN){					if(menu_cursor >= SIZE_GAME_MAIN)			menu_cursor = SIZE_GAME_MAIN;}
			else if(menu_state==GAME_SHOP){				if(menu_cursor >= SIZE_GAME_SHOP)			menu_cursor = SIZE_GAME_SHOP;}
			else if(menu_state==GAME_SHOP_WEAPONS){		if(menu_cursor >= SIZE_GAME_SHOP_WEAPONS)	menu_cursor = SIZE_GAME_SHOP_WEAPONS;}
			else if(menu_state==GAME_SHOP_SHIPS){		if(menu_cursor >= SIZE_GAME_SHOP_SHIPS)		menu_cursor = SIZE_GAME_SHOP_SHIPS;}
			else if(menu_state==GAME_SHOP_SELECT){		if(menu_cursor >= SIZE_GAME_SHOP_SELECT)	menu_cursor = SIZE_GAME_SHOP_SELECT;}
			else if(menu_state==GAME_SHOP_SELECT_WEAPON ||menu_state==GAME_SHOP_SELECT_SHIP){		if(menu_cursor >= zeile / 4)			menu_cursor = zeile / 4 ;if(menu_cursor <= 2)menu_cursor = 2;}
			else if(menu_state==GAME_SHOP_SELECT_WEAPON ||menu_state==GAME_SHOP_SELECT_SHIP){		if(menu_cursor >= zeile / 4)			menu_cursor = zeile / 4 ; if(menu_cursor <= 2)menu_cursor = 2;}
			else if(menu_state==GAME_OPTIONS){			if(menu_cursor >= SIZE_GAME_OPTIONS)		menu_cursor = SIZE_GAME_OPTIONS;}


			print("/",13,1 + 4 * menu_cursor);
			time_old = getMsTimer() + MENU_DELAY;
		}

		if(B_A && ( time >= time_old))
		{
			if(menu_state == GAME_MAIN)
			{
				menu_state = menu_cursor ;
				if(menu_state == GAME_SHOP)
				{
					menu_cursor = 2;
				}
				else menu_cursor = 1;
			}

			else if(menu_state == GAME_SHOP)
			{
				menu_state = menu_cursor + 30 -1;
				menu_cursor = 2;
			}

			//weapons: equipment_bought 0->7
			else if(menu_state == GAME_SHOP_WEAPONS)
			{
				uint8_t prize = 0;
				if(menu_cursor == 2) prize = PRICE_WEAPON_1;
				if(menu_cursor == 3) prize = PRICE_WEAPON_2;
				if(menu_cursor == 4) prize = PRICE_WEAPON_3;
				if(menu_cursor == 5) prize = PRICE_WEAPON_4;

				if( !(weapons_bought & (1<< (menu_cursor-1)))&& (points >=  prize) )
				{
					weapons_bought |= (1<<(menu_cursor-1));
					points -= prize;
				}
			}

			//ships: equipment_bought 8->15
			else if(menu_state == GAME_SHOP_SHIPS)
			{
				uint8_t prize = 0;
				if(menu_cursor == 2) prize = PRICE_SHIP_1;
				if(menu_cursor == 3) prize = PRICE_SHIP_2;
				if(menu_cursor == 4) prize = PRICE_SHIP_3;

				if( !(ships_bought & (1<< (menu_cursor-1)))&& (points >=  prize)  )
				{
					ships_bought |= (1<<(menu_cursor-1));
					points -= prize;
				}
			}
			else if(menu_state == GAME_SHOP_SELECT)
			{
				menu_state = menu_cursor + 32;
				menu_cursor = 2;
			}

			else if(menu_state == GAME_SHOP_SELECT_WEAPON)
			{

				//waffe zuordenen

			}
			else if(menu_state == GAME_SHOP_SELECT_SHIP)
			{
				//schiff zuordenen

			}

			else if(menu_state == GAME_OPTIONS)
			{
				menu_state = menu_cursor + 40;
			}

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();
			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}

		if(B_B && (time >= time_old))
		{
			if((menu_state == GAME_SHOP_WEAPONS )|| (menu_state == GAME_SHOP_SHIPS) || (menu_state == GAME_SHOP_SELECT))
			{
				menu_cursor = (menu_state % 10) +1;
				menu_state = GAME_SHOP;
			}
			else if(menu_state == GAME_SHOP_SELECT_WEAPON ||menu_state == GAME_SHOP_SELECT_SHIP)
			{
				menu_cursor = (menu_state - 2)%10;
				menu_state = GAME_SHOP_SELECT;
			}

			else if(menu_state != GAME_MAIN)
			{
				menu_cursor = menu_state % 10;
				if(menu_cursor<=1)menu_cursor =1;
				menu_state = GAME_MAIN;
			}

			time_old = getMsTimer() + MENU_DELAY;
			displayClear();
			if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
		}

		printN(menu_cursor,1,5);
		printN(menu_state,1,7);
		printB(weapons_bought,0,0);
		printB(ships_bought,100,0);
		printN(zeile,1,2);



		switch(menu_state)
		{
			case GAME_MAIN:
				print("Continue",20,5);
				print("Save",20,9);
				print("Shop",20,13);
				print("Options",20,17);
				break;

			case GAME_SAVE:
				//safe all to eeprom and go back to game menu
				menu_state = GAME_MAIN;
				menu_cursor = 2;
				print("/",13,1 + 4 * menu_cursor);
				print("Saved",20,9);
				break;

			case GAME_CONTINUE:
				//return new player data
				displayClear();
				return Player;



			case GAME_SHOP:
				print("SHOP",20,4);

				print("D",100,4);
				printN(points,110,4);
				print("select equipment",20,9);
				print("Buy Weapons",20,13 );
				print("buy Vessels",20,17 );
				break;

			case GAME_SHOP_SELECT:
				print("Equipment",20,4);
				print("D",100,4);
				printN(points,106,4);
				print("Change Weapon",20,9);
				print("Change SHIPtype",20,13 );

				break;

			case GAME_SHOP_SELECT_WEAPON:
				print("SELECTED:",20,4);
				print("GUN",110,4);
				zeile = 5;


				if( weapons_bought & (1<<0) & 0 /*and not selected*/)//normal gun
				{
					zeile += 4;
					print("GUN",20,zeile);

				}
				if( weapons_bought & (1<<1))//machinegun
				{
					zeile += 4;
					print("MACHINEGUN",20,zeile);

				}
				if( weapons_bought & (1<<2))//heavy
				{
					zeile += 4;
					print("HEAVY BOLTER",20,zeile);

				}
				if( weapons_bought & (1<<3))//rocket
				{
					zeile += 4;
					print("ROCKET",20,zeile);

				}
				if( weapons_bought & (1<<4))//shotgun
				{
					zeile += 4;
					print("SHOTGUN",20,zeile);

				}
				if(zeile == 5)
				{
					print("empty",20,9);
					print(" ",13,9);
				}


				break;

			case GAME_SHOP_SELECT_SHIP:

				print("SELECTED:",20,4);
				print("PATROL",100,4);
				zeile = 5;


				if( ships_bought & (1<<0) & 0 /*and not selected*/)//normal gun
				{
					zeile += 4;
					print("PATROL",20,zeile);

				}
				if( ships_bought & (1<<1))//machinegun
				{
					zeile += 4;
					print("LIGHT",20,zeile);

				}
				if( ships_bought & (1<<2))//heavy
				{
					zeile += 4;
					print("HEAVY",20,zeile);

				}
				if( ships_bought & (1<<3))//rocket
				{
					zeile += 4;
					print("DESTROYER",20,zeile);

				}
//				if( weapons_bought & (1<<4))//shotgun
//				{
//					print("SHOTGUN",20,zeile);
//					zeile += 4;
//				}

				if(zeile == 5)
				{
					print("empty",20,9);
					print(" ",13,9);

				}

				break;

			case GAME_SHOP_WEAPONS:

				print("WEAPONS",20,4);
				print("D",100,4);
				printN(points,110,4);
				print("Machine Gun",20,9);
				print("Heavy bolter",20,13 );
				print("Rocket",20,17 );
				print("Shotgun",20,21 );

				if( weapons_bought & (1<<1))//machinegun
				{
					print("Owned",110,9);
				}
				else
				{
					printN(PRICE_WEAPON_1,110,9);
				}

				if(  weapons_bought & (1<<2))//heavy bolter
				{
					print("Owned",110,13);
				}
				else
				{
					printN(PRICE_WEAPON_2,110,13);
				}

				if( weapons_bought & (1<<3))//rocket
				{
					print("Owned",110,17);
				}
				else
				{
					printN(PRICE_WEAPON_3,110,17);
				}

				if( weapons_bought & (1<<4))//shotgun
				{
					print("Owned",110,21);
				}
				else
				{
					printN(PRICE_WEAPON_4,110,21);
				}
				break;

			case GAME_SHOP_SHIPS:

				print("SHIPS",20,4);
				print("D",100,4);
				printN(points,110,4);
				print("LIGHT Cruiser",20,9);
				print("Heavy Cruiser",20,13 );
				print("Destroyer",20,17 );
				print("",20,21 );


				if( ships_bought & (1<<1))//machinegun
				{
					print("Owned",110,9);
				}
				else
				{
					printN(PRICE_SHIP_1,110,9);
				}

				if(  ships_bought & (1<<2))//heavy bolter
				{
					print("Owned",110,13);
				}
				else
				{
					printN(PRICE_SHIP_2 ,110,13);
				}

				if(  ships_bought & (1<<3))//rocket
				{
					print("Owned",110,17);
				}
				else
				{
					printN(PRICE_SHIP_3,110,17);
				}

				break;

			case GAME_OPTIONS:
				if( (set_options>>0) & 1)	print("Music ON",20,5);
				else	print("Music oFF",20,5);

				if((set_options>>1) & 1)	print("Invert Colours ON  ",20,9);
				else	print("Invert Colours OFF",20,9);

				break;

			case GAME_OPTIONS_1:
				if( (set_options>>0) & 1)	set_options &= ~(1<<0);
				else	set_options |= 1<<0;

				//disable music
				menu_state = GAME_OPTIONS;
				menu_cursor = 1;

				break;

			case GAME_OPTIONS_2:
				if((set_options>>1) & 1)	set_options &= ~(1<<1);
				else	set_options |= 1<<1;

				displayInverse((set_options>>1));
				menu_state = GAME_OPTIONS;
				menu_cursor = 2;
				break;
			default:
				print("fehler",130,1);
				printN(menu_cursor,130,3);
				printN(menu_state,130,5);
				menu_state = 0;
				menu_cursor = 0;

		}
	}
}

