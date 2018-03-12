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





//old code graveyard
/*
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
*/

uint8_t weapons_bought 	= 0b00000000;//from least significant bit starting with GUN, same order as weaponstext
uint8_t ships_bought 	= 0b00000001;//from least significant bit starting with GUN, same order as shipstext
uint8_t set_options 	= 0b00000001;//from lsb: music, invert display,

uint16_t points = 10000;


const char *duplicate_text[] = {"Continue","Save","SHOP","OPTIONS","D","/",//5
		"SELECT EQUIPMENT","BUY WEAPONS","BUY VESSELS","EQUIPMENT","CHANGE WEAPON",//10
		"CHANGE SHIPTYPE","SELECTED","A: GUN","B: ","empty",//15
		" ","GUN","MULTI","LAUNCHER","HEAVY",//20
		"NOPPY","SHOTGUN","MACHINEGUN","BOUNCE","          ",//25
			"PATROL","LIGHT","HEAVY","DESTROYER",};//30


const char *menu_text[] = {};
const char *pause_text[] = {};
const char *shop_text[] = {};

const char *weapons_text[] = {"GUN","MACHINEGUN","MULTISHOT","HEAVY","SHOTGUN","NOPPY","LAUNCHER","BOUNCE"};
const char *ships_text[] = {"PATROL","LIGHT","HEAVY","DESTROYER"};

uint8_t price_weapons[] = { 61,62,63,64,65,66,67 };
uint8_t price_ships[] =  { 51,52,53,54,55,56,57 };
uint8_t price_upgrade[] = {2,3,4,5,6};

//choose weapons/ships from inventory or upgrade
void shop_menu(void)
{
	displayClear();
	uint8_t menu_state = MAIN;
	uint8_t menu_cursor = 1;

	uint8_t iter =0;
	uint8_t zeile = 9;
	uint8_t next_col = 0;
	uint8_t scroll_start=0;
	uint8_t counter = 0;
	uint8_t lower_boundary = 0;
	uint8_t upgrade_selector = 0;
	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;

	//print cursor on startpos neeed?
	print("/",13,1 + 4 * menu_cursor);

	while(1){

	time = getMsTimer();

	printB(weapons_bought,20,24);
	printB(ships_bought,120,24);
	printN(menu_cursor,1,6);
	printN(menu_state,1,8);
	printN(scroll_start,1,1);
	printN(time,1,1);
	printN(iter,1,10);
	printN(lower_boundary,1,12);


	if( B_UP && ( time >= time_old))//control upper menu boundary
	{
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor-=1;

		if(menu_state == MAIN)//limit upper boundary
		{
			if(menu_cursor <= MAIN_UPPER_BOUNDARY)
			{
				menu_cursor = MAIN_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == INVENTORY)//limt upper boundary
		{
			if(menu_cursor <= IVENTORY_UPPER_BONDARY)
			{
				menu_cursor = IVENTORY_UPPER_BONDARY;
			}
		}
		else if(menu_state == INVENTORY_WEAPONS)
		{
			if(menu_cursor <= INVENTORY_WEAPONS_UPPER_BOUNDARY)
			{
				menu_cursor = INVENTORY_WEAPONS_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == INVENTORY_SHIPS)
		{
			if(menu_cursor <= INVENTORY_SHIPS_UPPER_BOUNDARY)
			{
				menu_cursor = INVENTORY_SHIPS_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == SHOP)
		{
			if(menu_cursor <= SHOP_UPPER_BOUNDARY)
			{
				menu_cursor = SHOP_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == SHOP_WEAPONS)
		{
			if(menu_cursor <= SHOP_WEAPONS_UPPER_BOUNDARY)
			{
				menu_cursor = SHOP_WEAPONS_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == SHOP_SHIPS)
		{
			if(menu_cursor <= SHOP_SHIPS_UPPER_BOUNDARY)
			{
				menu_cursor = SHOP_SHIPS_UPPER_BOUNDARY;
			}
		}
		else if(menu_state == SHOP_UPGRADE_WEAPON)
		{
			if(menu_cursor <= SHOP_UPGRADE_WEAPON_UPPER_BOUNDARY)
			{
				menu_cursor = SHOP_UPGRADE_WEAPON_UPPER_BOUNDARY;
			}
		}
		else if(menu_cursor <= 1)
		{
			menu_cursor = 1;
		}

		if(menu_cursor >= 5)//if menu cursor >= 5 , always on lower screen
		{
			print("/",13,21);
		}
		else
		{
			print("/",13,1 + 4 * menu_cursor);
		}

		time_old = getMsTimer() + MENU_DELAY;
	}



	if( B_DOWN && ( time >= time_old))//control lower menu boundary
	{

		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor+=1;

		if(menu_state == MAIN)//keep menucursor from going off screen
		{
			if(menu_cursor >= MAIN_LOWER_BOUNDARY)
			{
				menu_cursor = MAIN_LOWER_BOUNDARY;
			}
			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == INVENTORY)//keep menucursor from going off screen
		{
			if(menu_cursor >= INVENTORY_LOWER_BOUNDARY)
			{
				menu_cursor = INVENTORY_LOWER_BOUNDARY;
			}
			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == INVENTORY_WEAPONS)//scrollable display with menu cursor
		{
			if(menu_cursor <=5)
			{
				counter = 0;
				lower_boundary = 1;
				while(counter <= 7)
				{
					if(weapons_bought & (1<<counter))
					{
						lower_boundary++;
					}
					counter++;
				}
				if(lower_boundary >= 8)
				{
					lower_boundary = 8;
				}
				if(menu_cursor >= lower_boundary )
				{
					menu_cursor = lower_boundary;
				}
				if(menu_cursor <= 2)
				{
					menu_cursor = 2;
				}
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 6 )//highest possible menu_cursor value
			{
				print("/",13,21);//when the menu is scrolling the cursor allways sits on the lowest position
			}

			if(menu_cursor >=8)
			{
				menu_cursor = 8;
			}

		}
		else if(menu_state == INVENTORY_SHIPS)
		{
			if(menu_cursor <=5)
			{
				counter = 0;
				lower_boundary = 1;
				while(counter <= 7)
				{
					if(ships_bought & (1<<counter))
					{
						lower_boundary++;
					}
					counter++;
				}
				if(lower_boundary >= 5)
				{
					lower_boundary = 5;
				}
				if(menu_cursor >= lower_boundary )
				{
					menu_cursor = lower_boundary;
				}
				if(menu_cursor <= 2)
				{
					menu_cursor = 2;
				}
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 5 )//highest possible menu_cursor value
			{
				menu_cursor = 5;
				print("/",13,21);//when the menu is scrolling the cursor allways sits on the lowest position
			}
			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == SHOP)
		{

			if(menu_cursor >= SHOP_LOWER_BOUNDARY)
			{
				menu_cursor = SHOP_LOWER_BOUNDARY;
			}

			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == SHOP_WEAPONS)//scrollable display with menu cursor
		{
			if(menu_cursor <=5)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 6 && menu_cursor <=9)//highest possible menu_cursor value
			{
				print("/",13,21);//when the menu is scrolling the cursor allways sits on the lowest position
			}
			else
			{
				menu_cursor = 9;
				print("/",13,21);
			}
		}
		else if(menu_state == SHOP_SHIPS)//scrollable display with menu cursor
		{
			if(menu_cursor <=5)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 6 )//highest possible menu_cursor value
			{
				menu_cursor = 6;
				print("/",13,21);//when the menu is scrolling the cursor allways sits on the lowest position
			}
		}

		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_A && ( time >= time_old))
	{
		displayClear();
		if(menu_state == MAIN)
		{
			menu_state = menu_cursor-1;
			menu_cursor = 2;
		}
		else if(menu_state == SHOP)
		{
			if(menu_cursor == 2)
			{
				menu_cursor = 2;
				menu_state = SHOP_WEAPONS;
			}
			else if(menu_cursor == 3)
			{
				menu_state = SHOP_SHIPS;
				menu_cursor = 2;
			}
		}
		else if(menu_state == INVENTORY)
		{
			if(menu_cursor == 2)
			{
				menu_state = INVENTORY_WEAPONS;
				menu_cursor = 2;
			}
			if(menu_cursor ==3)
			{
				menu_state = INVENTORY_SHIPS;
				menu_cursor = 2;
			}
		}
		else if(menu_state == SHOP_WEAPONS)
		{
			if(menu_cursor-3 >= 0){
			if((weapons_bought & (1<<(menu_cursor-3))) == 0)
			{

				if(price_weapons[menu_cursor -3]<= points)
				{
					points = points - price_weapons[menu_cursor-3];
					weapons_bought |= (1<<(menu_cursor-3));
				}
			}}
			if(menu_cursor >=5)
			{

				print("/",13,21);
			}
		}
		else if(menu_state == SHOP_SHIPS)
		{
			if(menu_cursor-3 >= 0){
			if((ships_bought & (1<<(menu_cursor-2))) == 0)
			{

				if(price_ships[menu_cursor -2]<= points)
				{
					points = points - price_ships[menu_cursor-2];
					ships_bought |= (1<<(menu_cursor-2));
				}
			}}
			if(menu_cursor >=5)
			{

				print("/",13,21);
			}
		}
		else if(menu_state == INVENTORY_SHIPS){}
		else if(menu_state == INVENTORY_WEAPONS){}
		else if(menu_state == SHOP_UPGRADE_WEAPON){}
		else
		{

		}
		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}
	if( B_B && ( time >= time_old))
	{
		displayClear();
		if(menu_state == MAIN)
		{
			// do nothing
			return;

		}
		else if(menu_state == INVENTORY)
		{
			menu_state = MAIN;
			menu_cursor = INVENTORY + 1;

		}
		else if(menu_state == INVENTORY_WEAPONS)
		{
			menu_state = INVENTORY;
			menu_cursor = 2;
		}
		else if(menu_state == INVENTORY_SHIPS)
		{
			menu_state = INVENTORY;
			menu_cursor = 3;
		}
		else if(menu_state == SHOP)
		{
			menu_state = MAIN;
			menu_cursor = SHOP +1;
		}
		else if(menu_state == SHOP_WEAPONS)
		{
			menu_state = SHOP;
			menu_cursor = 2;
		}
		else if(menu_state == SHOP_SHIPS)
		{
			menu_state = SHOP;
			menu_cursor = 3;
		}
		else if(menu_state == SHOP_UPGRADE_WEAPON)
		{
			menu_state = SHOP_WEAPONS;
			menu_cursor = upgrade_selector;
		}
		else
		{
			//do nothing
		}


		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}



	switch(menu_state)
	{
		case MAIN:
			print("CONTINUE",20,5);
			print("INVENTORY",20,9);
			print("SHOP",20,13);
			print("SAVE",20,17);
			print("QUIT",20,21);
			break;
		case INVENTORY:
			print("INVENTORY",20,5);
			print("WEAPONS",20,9);
			print("SHIPS",20,13);
			break;
		case INVENTORY_WEAPONS://show all bought weapons
			print("SELSECUNDARY:",20,4);
			print("EXAMPLE",90,4);

			iter =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (weapons_bought & (1<<iter)))
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(1/*selected*/)
						{
							print("current",100,zeile);
						}
						zeile +=4;
					}
					iter++;
				}
			}
			//scroll when the cursor would run off the screen
			else if(menu_cursor >= 6)
			{
				scroll_start = menu_cursor -5;
				//go through weapons_bought
				while(iter<=7 )
				{
					// display all bought weapons until screen is full starting at scroll_starts
					if(zeile <= 21 && (weapons_bought & (1<<iter)) && iter >= scroll_start)
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(1/*selected*/)
						{
							print("current",100,zeile);
						}
						zeile +=4;
						scroll_start++;
					}
					iter++;
				}
			}
			//noting bought when zeile didnt increase
			if(zeile == 9)
			{
				print("empty",20,9);
				print(" ",13,9);
			}
			break;
		case INVENTORY_SHIPS:
			print("SHIP:",20,4);
			print("EXAMPLE",90,4);

			iter =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (ships_bought & (1<<iter)))
					{
						print("          ",20,zeile);
						print(ships_text[iter],20,zeile);
						if(1/*selected*/)
						{
							print("current",100,zeile);
						}
						zeile +=4;
					}
					iter++;
				}
			}
			//scroll when the cursor would run off the screen
			else if(menu_cursor >= 6)
			{
				menu_cursor = 6;
			}

			if(zeile == 9)
			{
				print("empty",20,9);
				print(" ",13,9);
			}
			break;
		case SHOP:
			print("SHOP",20,5);
			print("BUY WEAPONS",20,9);
			print("BUY SHIPS",20,13);
			break;
		case SHOP_WEAPONS:
			print("WEAPONS",20,5);
			printN(points,100,5);

			next_col =0;
			zeile = 9;
			scroll_start = 0;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
					while(zeile <= 21 )
					{
						print("          ",20,zeile);
						print(weapons_text[scroll_start],20,zeile);

						if(scroll_start ==0 )
						{
							print("       ",100,zeile);
							print("UPGRADE",100,zeile);
						}
						else if((weapons_bought & (1<<(scroll_start-1))))
						{
							print("       ",100,zeile);
							print("UPGRADE",100,zeile);
						}
						else
						{
							print("       ",100,zeile);
							//printN(scroll_start,100,zeile);
							printN(price_weapons[scroll_start-1],100,zeile);
						}

						zeile +=4;
						scroll_start++;
					}
			}
			//scroll when the cursor would run off the screen
			else if(menu_cursor >= 6)
			{
				if(menu_cursor == 8)
				{
					scroll_start = 3;
				}
				else
				{
					scroll_start = menu_cursor - 5;
				}

				// display all bought ships until screen is full starting at scroll_starts
				while(zeile <= 21 && menu_cursor >= scroll_start+1)
				{
					print("          ",20,zeile);
					print(weapons_text[scroll_start],20,zeile);

					if(weapons_bought & (1<<(scroll_start-1)))
					{
						print("       ",100,zeile);
						print("UPGRADE",100,zeile);
					}
					else
					{
						print("       ",100,zeile);
						//printN(scroll_start,100,zeile);
						printN(price_weapons[scroll_start-1],100,zeile);
					}
					zeile +=4;
					scroll_start++;
				}
			}

			//nothing bought when zeile didnt increase
			if(zeile == 9)
			{
				print("empty",20,9);
				print(" ",13,9);
			}
			break;
		case SHOP_SHIPS:
			print("SHIPS",20,5);
			printN(points,100,5);


			next_col =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
					while(zeile <= 21 )
					{
						print("          ",20,zeile);
						print(ships_text[next_col],20,zeile);

						if(ships_bought & (1<<next_col))
						{
							print("          ",100,zeile);
							print("UPGRADE",100,zeile);
						}
						else
						{
							print("          ",100,zeile);
							printN(price_ships[next_col],100,zeile);
						}
						zeile +=4;
						next_col ++;
					}
			}
			//scroll when the cursor would run off the screen
			else if(menu_cursor >= 5)
			{
				menu_cursor = 5;
			}

			break;
		case SHOP_UPGRADE_WEAPON:
			break;
		case SAVE:
			menu_state = MAIN;
			menu_cursor = 4;
			//save state to selected safe game
			break;
		case QUIT:
			//return to start screen
			menu_state = MAIN;
			menu_cursor = 5;
			break;

	}
}

}

void pause_menu(void)
{
	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;
	uint8_t menu_state = MAIN;
	uint8_t menu_cursor = 2;
	print("/",13,1 + 4 * menu_cursor);
	while(1){

		time = getMsTimer();

	if( B_UP && ( time >= time_old))
	{
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor-=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor <=2)
			{
				menu_cursor = 2;
			}
		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor <= 2)
			{
				menu_cursor = 2;
			}
		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}

		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_DOWN && ( time >= time_old))
	{
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor+=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor >= 5)
			{
				menu_cursor = 5;
			}

		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor >= 3)
			{
				menu_cursor = 3;
			}
		}
		else
		{
			menu_cursor = 2;
			menu_state = MAIN;
		}

		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_A && ( time >= time_old))
	{
		if(menu_state == MAIN)
		{
			menu_state = menu_cursor -1;
			menu_cursor = 2;
		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor == 2) // music
			{
				menu_cursor = 2;
				menu_state = OPTIONS_1;
			}
			if(menu_cursor == 3) // invert
			{
				menu_state = OPTIONS_2;
				menu_cursor = 3;
			}
		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}
		displayClear();
		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_B && ( time >= time_old))
	{
		if(menu_state == OPTIONS)
		{
			menu_state = MAIN;
			menu_cursor = 4;
		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}

		displayClear();
		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}


	switch(menu_state)
	{
		case MAIN:
			print("PAUSE",20,5);
			print("Continue",20,9);
			print("SAVE GAME",20,13);
			print("OPTIONS",20,17);
			print("QUIT",20,21);
			break;

		case CONTINUE:
			//do sth
			print("Cont",1,1);
			while(1);
			break;

		case SAVE2:
			//do sth
			print("SAVE",1,1);
			while(1);
			break;

		case OPTIONS:
			print("OPTIONS",20,5);
			print("Music",20,9);
			if( set_options & (1<<0))
			{
				print("ON",50,9);
			}
			else
			{
				print("OFF",50,9);
			}

			print("Invert Colours",20,13);
			if( set_options & (1<<1))
			{
				print("ON  ",106,13);
			}
			else
			{
				print("OFF",106,13);
			}
			break;

		case OPTIONS_1:
			if( set_options & (1<<0))
			{
				set_options &= ~(1<<0);
			}
			else
			{
				set_options |= (1<<0);
			}
			//disable music
			//so sth
			menu_state = OPTIONS;
			menu_cursor = 2;

			break;

		case OPTIONS_2:
			if( set_options & (1<<1))
			{
				set_options &= ~(1<<1);
			}
			else
			{
				set_options |= (1<<1);
			}
			displayInverse(set_options & (1<<1));
			menu_state = OPTIONS;
			menu_cursor = 3;
			break;

		case QUIT:
			//do sth
			print("QUIT",1,1);
			while(1);
			break;
	}
}
}


void main_menu()
{
	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;
	uint8_t menu_state = MAIN;
	uint8_t menu_cursor = 2;
	print("/",13,1 + 4 * menu_cursor);
	while(1){

		time = getMsTimer();

	if( B_UP && ( time >= time_old))
	{
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor-=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor <=1)
			{
				menu_cursor = 1;
			}
		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor <= 2)
			{
				menu_cursor = 2;
			}
		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}

		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_DOWN && ( time >= time_old))
	{
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor+=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor >= 4)
			{
				menu_cursor = 4;
			}

		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor >= 3)
			{
				menu_cursor = 3;
			}
		}
		else
		{
			menu_cursor = 2;
			menu_state = MAIN;
		}

		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_A && ( time >= time_old))
	{
		if(menu_state == MAIN)
		{
			menu_state = menu_cursor -1;
			menu_cursor = 2;
		}
		else if(menu_state == SELECTGAME)
		{
			menu_state = menu_cursor + 20;
		}
		else if(menu_state == OPTIONS)
		{
			if(menu_cursor == 2) // music
			{
				menu_cursor = 2;
				menu_state = OPTIONS_1;
			}
			if(menu_cursor == 3) // invert
			{
				menu_state = OPTIONS_2;
				menu_cursor = 3;
			}
		}
		else if(menu_state == HIGHSCORES)
		{
			//do nothing
			menu_state = HIGHSCORES;
			menu_cursor = 2;

		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}
		displayClear();
		if(menu_state != HIGHSCORES)
		{
			print("/",13,1 + 4 * menu_cursor);
		}
		time_old = getMsTimer() + MENU_DELAY;
	}

	if( B_B && ( time >= time_old))
	{
		if(menu_state == OPTIONS)
		{
			menu_state = MAIN;
			menu_cursor = 4;
		}
		if(menu_state == SELECTGAME)
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}
		if(menu_state == HIGHSCORES)
		{
			menu_state = MAIN;
			menu_cursor = 3;
		}
		else
		{
			menu_state = MAIN;
			menu_cursor = 2;
		}

		displayClear();
		print("/",13,1 + 4 * menu_cursor);
		time_old = getMsTimer() + MENU_DELAY;
	}


	switch(menu_state)
	{
		case MAIN:
			print("CONTINUE",20,5);
			print("SELECT GAME",20,9);
			print("HIGHSCORES",20,13);
			print("OPTIONS",20,17);
			break;

		case CONTINUE:
			//do sth
			print("Cont",1,1);
			while(1);
			break;

		case SELECTGAME:
			print("SELECT GAME",20,5);
			print("SAVE1",20,9);
			print("SAVE2",20,9);
			print("SAVE3",20,9);
			break;

		case SELECTGAME_1:
			print("sAVE1",1,1);
			while(1);
			break;
		case SELECTGAME_2:
			print("sAVE1",1,1);
			while(1);
			break;
		case SELECTGAME_3:
			print("sAVE1",1,1);
			while(1);
			break;

		case HIGHSCORES:
			print("HIGHSCORES",20,5);
			print("SAVE1",20,9);
			print("SAVE2",20,13);
			print("SAVE3",20,17);
			break;


		case OPTIONS2:
			print("OPTIONS",20,5);
			print("Music",20,9);
			if( set_options & (1<<0))
			{
				print("ON",50,9);
			}
			else
			{
				print("OFF",50,9);
			}

			print("Invert Colours",20,13);
			if( set_options & (1<<1))
			{
				print("ON  ",106,13);
			}
			else
			{
				print("OFF",106,13);
			}
			break;

		case OPTIONS_1:
			if( set_options & (1<<0))
			{
				set_options &= ~(1<<0);
			}
			else
			{
				set_options |= (1<<0);
			}
			//disable music
			//so sth
			menu_state = OPTIONS2;
			menu_cursor = 2;

			break;

		case OPTIONS_2:
			if( set_options & (1<<1))
			{
				set_options &= ~(1<<1);
			}
			else
			{
				set_options |= (1<<1);
			}
			displayInverse(set_options & (1<<1));
			menu_state = OPTIONS2;
			menu_cursor = 3;
			break;
	}
}
}
