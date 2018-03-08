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


uint8_t weapons_bought = 0;
uint8_t ships_bought = 0;
uint16_t points = 1000;
uint8_t scroll =1;
uint8_t zeile = 5;

const char *duplicate_text[] = {"Continue","Save","SHOP","OPTIONS","D","/",//5
		"SELECT EQUIPMENT","BUY WEAPONS","BUY VESSELS","EQUIPMENT","CHANGE WEAPON",//10
		"CHANGE SHIPTYPE","SELECTED","A: GUN","B: ","empty",//15
		" ","GUN","MULTI","LAUNCHER","HEAVY",//20
		"NOPPY","SHOTGUN","MACHINEGUN","BOUNCE","          ",//25
			"PATROL","LIGHT","HEAVY","DESTROYER",};//30


uint8_t price_weapons[7] = { 80,90,100,110,120,130,150 };
uint8_t price_ships[7] =  { 81,91,101,111,121,131,151 };


Object display_gamemenu(Object Player)//not finished
{
	displayClear();
	uint8_t menu_state = GAME_MAIN;
	uint8_t menu_cursor = 1;


	uint32_t time = 0;
	uint32_t time_old = time + MENU_DELAY;
	uint8_t zeile = 9;
	print("/",13,1 + 4 * menu_cursor);

	uint8_t iter = 0;

	uint8_t cursor_stop_ship;
	uint8_t cursor_stop_weapon;


	while(1){
		time = getMsTimer();
		uint8_t counter = 0;


		if( B_UP && ( time >= time_old))//move cursor up
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor-=1;

			if((menu_state == GAME_SHOP || menu_state == GAME_SHOP_SELECT) && (menu_cursor <=2))
			{
				menu_cursor = 2;
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_state == GAME_SHOP_WEAPONS ||menu_state == GAME_SHOP_SHIPS|| menu_state == GAME_SHOP_SELECT_WEAPON || menu_state == GAME_SHOP_SELECT_SHIP)
			{
				if(menu_cursor <=2) menu_cursor = 2;
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor <= 1)
			{
				menu_cursor = 1;
				print("/",13,1 + 4 * menu_cursor);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}


			time_old = getMsTimer() + MENU_DELAY;

		}
		if( B_DOWN && ( time >= time_old))//move cursor up
		{
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor+=1;

			//lower cursor boundaries
			if(menu_state==GAME_MAIN){					if(menu_cursor >= SIZE_GAME_MAIN)			menu_cursor = SIZE_GAME_MAIN;}
			else if(menu_state==GAME_SHOP){				if(menu_cursor >= SIZE_GAME_SHOP)			menu_cursor = SIZE_GAME_SHOP;}
			else if(menu_state==GAME_SHOP_WEAPONS)
			{
				if(menu_cursor <= 5){
					if(menu_cursor <= 2) menu_cursor = 2;
					print("/",13,1 + 4 * menu_cursor);
				}
				else if(menu_cursor <= 8)
				{
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}
				else
				{
					menu_cursor = 8;
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}

			}
			else if(menu_state==GAME_SHOP_SHIPS)
			{
				if(menu_cursor >=5) menu_cursor = 5;
				if(menu_cursor <= 5){
					if(menu_cursor <= 2) menu_cursor = 2;
					print("/",13,1 + 4 * menu_cursor);
				}
				else if(menu_cursor <= 8)
				{
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}
				else
				{
					menu_cursor = 8;
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}

			}

			else if(menu_state==GAME_SHOP_SELECT){		if(menu_cursor >= SIZE_GAME_SHOP_SELECT)	menu_cursor = SIZE_GAME_SHOP_SELECT;}

			else if(menu_state==GAME_SHOP_SELECT_WEAPON )
//			{
//				cursor_stop_weapon =0;
//				for(counter = 0; counter < 8;counter++)
//				{
//					if( weapons_bought & (1<<counter))
//					{
//						cursor_stop_weapon++;
//					}
//				}
//
//				if(cursor_stop_weapon < menu_cursor)
//				{
//					menu_cursor = cursor_stop_weapon+1;
//					if(menu_cursor <= 5)
//					{
//						if(menu_cursor <= 2)
//						{
//							menu_cursor = 2;
//						}
//						print("/",13,1 + 4 * menu_cursor);
//					}
//
//				}
//
//			}
			{
				if(menu_cursor <= 5){
					if(menu_cursor <= 2) menu_cursor = 2;
					print("/",13,1 + 4 * menu_cursor);
				}
				else if(menu_cursor <= 8)
				{
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}
				else
				{
					menu_cursor = 8;
					print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
				}
			}



			else if(menu_state==GAME_SHOP_SELECT_SHIP)
			{
				cursor_stop_ship =0;
				for(counter = 0; counter < 8;counter++)
				{
					if( ships_bought & (1<<counter))
					{
						cursor_stop_ship++;
					}
				}

				if(cursor_stop_ship <= menu_cursor)
				{
					menu_cursor = cursor_stop_ship+1
							;
					if(menu_cursor <= 5)
					{
						if(menu_cursor <= 2) menu_cursor = 2;
						print("/",13,1 + 4 * menu_cursor);
					}
					else if(menu_cursor <= 8)
					{
						print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
					}
					else
					{
						menu_cursor = 8;
						print("/",13,21);//cursor unterer bildschirm rand wenn beim scrllen menu_cursor höher als anzeige
					}
				}
			}
			else if(menu_state==GAME_OPTIONS){			if(menu_cursor >= SIZE_GAME_OPTIONS)		menu_cursor = SIZE_GAME_OPTIONS;}


			if(menu_state != GAME_SHOP_SELECT_WEAPON||GAME_SHOP_SELECT_SHIP||GAME_SHOP_WEAPONS)print("/",13,1 + 4 * menu_cursor);
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
				if( !(weapons_bought & (1<< (menu_cursor-2)))&& (points >=  price_weapons[menu_cursor-2]) )
				{
					weapons_bought |= (1<<(menu_cursor-2));
					points -= price_weapons[menu_cursor-2];
				}
				else if( (weapons_bought & (1<< (menu_cursor-2))))
				{
					menu_state = GAME_SHOP_UPGRADE_WEAPON;
				}
			}

			//ships: equipment_bought 8->15
			else if(menu_state == GAME_SHOP_SHIPS)
			{
				if( !(ships_bought & (1<< (menu_cursor-2)))&& (points >=  price_ships[menu_cursor-2]) )
				{
					ships_bought |= (1<<(menu_cursor-2));
					points -= price_ships[menu_cursor-2];
				}
				else if( (ships_bought & (1<< (menu_cursor-2))))
				{
					menu_state = GAME_SHOP_UPGRADE_SHIP;
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
			if(menu_state == GAME_SHOP_WEAPONS)print("/",13,1 + 4 * menu_cursor);
			else if(menu_state !=MEN_HIGHSCORES)print("/",13,1 + 4 * menu_cursor);
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
	//	printN(menu_state,1,7);
	    printB(weapons_bought,0,23);
		printB(ships_bought,100,23);
//		printN(zeile,1,2);
//		printN(scroll,40,2);

		print(" ",13,1);

		switch(menu_state)
		{

			case GAME_SHOP_UPGRADE_WEAPON:
			//  menu_cursor selects the upgrade weapon


			break;

			case GAME_MAIN:
				print(duplicate_text[0],20,5);
				print(duplicate_text[1],20,9);
				print(duplicate_text[2],20,13);
				print(duplicate_text[3],20,17);
				break;

			case GAME_SAVE:
				//safe all to eeprom and go back to game menu
				menu_state = GAME_MAIN;
				menu_cursor = 2;
				print(duplicate_text[5],13,1 + 4 * menu_cursor);
				print("ed",20,27);
				break;

			case GAME_CONTINUE:
				//return new player data
				displayClear();
				return Player;

			case GAME_SHOP:
				print(duplicate_text[0],20,4);
				print(duplicate_text[4],100,4);
				printN(points,110,4);
				print(duplicate_text[6],20,9);
				print(duplicate_text[7],20,13 );
				print(duplicate_text[8],20,17 );
				break;

			case GAME_SHOP_SELECT:
				print(duplicate_text[9],20,4);
				print(duplicate_text[4],100,4);
				printN(points,106,4);
				print(duplicate_text[10],20,9);
				print(duplicate_text[11],20,13 );

				break;

			case GAME_SHOP_SELECT_WEAPON:
				print("SECUNDARY:",20,4);
				print("bsp",100,4);
				printN(points,110,4);

				zeile = 9;
				scroll =2; //first 0 weapon gun always selected


				printN(scroll,1,1);

				if(menu_cursor <= 5)
				{
					while(scroll <= 5 && iter < 8)
					{
						if((weapons_bought & (1<<iter)))
						{
							if(zeile < 22){
								print("          ",20,zeile);
								print(duplicate_text[16+scroll],20,zeile);
								if( weapons_bought & (1<<scroll-2))//machinegun
								{
									print("UPGRADE",110,zeile);
								}
								else
								{
									print("       ",110,zeile);
									printN(price_weapons[scroll-1],110,zeile);
								}

							}
							zeile +=4;
							scroll++;
						}
						iter++;
					}
				}

				else if(menu_cursor >=6)
				{
					if(menu_cursor == 8)scroll = 5;
					else scroll = (menu_cursor % 4) +1;

					while(scroll <= menu_cursor && iter < 8)
					{
						if((weapons_bought & (1<<iter))){
							if(zeile < 22)
							{
								print("          ",20,zeile);
								print(duplicate_text[16+scroll],20,zeile);
								if( weapons_bought & (1<<scroll-2))//machinegun
								{
									print("UPGRADE",110,zeile);
								}
								else
								{
									print("       ",110,zeile);
									printN(price_weapons[scroll-1],110,zeile);
								}

							}
							zeile +=4;
							scroll++;
						}
						iter++;
					}

				}
				if(zeile == 9)print("empty",20,9);

				printN(iter,1,1);
				printN(zeile,1,3);
				printN(menu_cursor,1,5);
				break;


//				print("Secondary",20,4);
//				print("B: GUN",90,4);
//
//				zeile = 9;
//				scroll =2;
//				iter =0;
//
//				if(menu_cursor <= 5)
//				{
//					while(scroll <= 5 && iter < 8)
//					{
//						if((weapons_bought & (1<<iter))){
//							if(zeile < 30)
//							{
//								print("          ",20,zeile);
//								print(duplicate_text[18+iter],20,zeile);
//							}
//
//							zeile +=4;
//							scroll++;
//						}
//						iter++;
//					}
//				}
//
//
//				else if(menu_cursor >=6)
//				{
//					if(menu_cursor == 8)scroll = 5;
//					else scroll = (menu_cursor % 4) +1;
//
//					while(scroll <= menu_cursor && iter < 8)
//					{
//						if((weapons_bought & (1<<iter))){
//							if(zeile < 22 )
//							{
//								print("          ",20,zeile);
//								print(duplicate_text[18+iter],20,zeile);
//							}
//							zeile +=4;
//							scroll++;
//						}
//					iter++;
//					}
//
//				}
//				if(zeile == 9)print("empty",20,9);
//				break;


			case GAME_SHOP_SELECT_SHIP:

				print("SELECTED",20,4);
				print("BSP SHIP",80,4);

				zeile = 9;
				scroll =2;
				iter =0;

				if(menu_cursor <= 5)
				{
					while(scroll <= 5 && iter < 8)
					{
						if((ships_bought & (1<<iter))){
							if(zeile < 22 )
							{
									print("          ",20,zeile);
									print(duplicate_text[26+iter],20,zeile);
							}
							zeile +=4;
							scroll++;
						}
						iter++;
					}
				}

				else if(menu_cursor >=6)
				{
					if(menu_cursor == 8)scroll = 5;
					else scroll = (menu_cursor % 4) +1;

					while(scroll <= menu_cursor && iter < 8)
					{
						if((ships_bought & (1<<iter))){
							if(zeile < 22 )
							{
								printN(scroll-1,20,zeile);
								if( ships_bought & (1<<scroll-2))//machinegun
								{
								print("UPGRADE",110,zeile);
								}
								else
								{
									print("       ",110,zeile);
									printN(price_ships[scroll-1],110,zeile);
								}
								zeile +=4;
								scroll++;
							}
							iter++;
						}
					}
				}
				if(zeile == 9)print("empty",20,9);
				break;


			case GAME_SHOP_WEAPONS:

				print("WEAPONS",20,4);
				print("D",100,4);
				printN(points,110,4);

				zeile = 9;
				scroll =2; //first 0 weapon gun always selected

				printN(scroll,1,1);
				if(menu_cursor <= 5)
				{
					while(scroll <= 5 )
					{
						if(zeile < 22){
						print("          ",20,zeile);
						print(duplicate_text[16+scroll],20,zeile);
						if( weapons_bought & (1<<scroll-2))//machinegun
						{
							print("UPGRADE",110,zeile);
						}
						else
						{
							print("       ",110,zeile);
							printN(price_weapons[scroll-1],110,zeile);
						}
						}
						zeile +=4;
						scroll++;
					}
				}

				else if(menu_cursor >=6)
				{
					if(menu_cursor == 8)scroll = 5;
					else scroll = (menu_cursor % 4) +1;

					while(scroll <= menu_cursor)
					{
						if(zeile < 22)
						{
							print("          ",20,zeile);
							print(duplicate_text[16+scroll],20,zeile);
							if( weapons_bought & (1<<scroll-2))//machinegun
							{
								print("UPGRADE",110,zeile);
							}
							else
							{
								print("       ",110,zeile);
								printN(price_weapons[scroll-1],110,zeile);
							}
							zeile +=4;
							scroll++;
						}
					}
				}

				break;

			case GAME_SHOP_SHIPS:

				print("SHIPS",20,4);
				print("D",100,4);
				printN(points,110,4);

				zeile = 9;
				scroll =2; //first 0 weapon gun always selected

				if(menu_cursor <= 5)
				{
					while(scroll <= 5 )
					{
						if(zeile < 22){
						print("          ",20,zeile);
						print(duplicate_text[24+scroll],20,zeile);
						if( ships_bought & (1<<scroll-2))//machinegun
						{
							print("UPGRADE",110,zeile);
						}
						else
						{
							print("       ",110,zeile);
							printN(price_ships[scroll-1],110,zeile);
						}
						}
						zeile +=4;
						scroll++;
					}
				}

				else if(menu_cursor >=6)
				{
					if(menu_cursor == 8)scroll = 5;
					else scroll = (menu_cursor % 4) +1;

					while(scroll <= menu_cursor)
					{
						if(zeile < 22)
						{
							print("          ",20,zeile);
							print(duplicate_text[24+scroll],20,zeile);
							if( ships_bought & (1<<scroll-2))//machinegun
							{
								print("UPGRADS",110,zeile);
							}
							else
							{
								print("       ",110,zeile);
								printN(price_ships[scroll-1],110,zeile);
							}
							zeile +=4;
							scroll++;
						}
					}
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

