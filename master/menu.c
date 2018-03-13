#include "buttons.h"
#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "savegame.h"
#include "environment.h"
#include "object.h"
#include "entities/stats.h"
#include <avr/eeprom.h>



uint8_t set_options 	= 0b00000001;

const char *menu_text[] = {};
const char *pause_text[] = {};
const char *shop_text[] = {};

const char *weapons_text[] = {"GUN","MACHINEGUN","MULTISHOT","HEAVY","SHOTGUN","NOPPY","LAUNCHER","BOUNCE"};
const char *ships_text[] = {"PATROL","LIGHT","HEAVY","DESTROYER"};

uint8_t price_weapons[] = { 61,62,63,64,65,66,67 };
uint8_t price_ships[] =  { 51,52,53,54,55,56,57 };
uint8_t price_upgrade[] = {2,3,4,5,6};
uint8_t weapon = 0;

//choose weapons/ships from inventory or upgrade
GameState shop_menu(Environment env)
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

	uint32_t time_old = env->time + MENU_DELAY;

	//print cursor on startpos neeed?
	print("/",13,1 + 4 * menu_cursor);

	while(1){

//	printB(env->gameState->boughtShip ,130,1);
//	printB(env->gameState->boughtWeapon,130,3);
//	printN(env->gameState->selShip ,130,5);
//	printN(env->gameState->selWeapon,130,7);
//
 	printN(menu_state,130,10);
	printN(menu_cursor,130,12);


	updateEnvironment(env);

	if( (env->buttons & M_U) && ( env->time >= time_old))//control upper menu boundary
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
		else if(menu_state == SHOP_UPGRADE_WEAPON)//scrollable display with menu cursor
		{
			if(menu_cursor <= 2)
			{
				menu_cursor = 2;
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

		time_old = env->time + MENU_DELAY;
	}

	if( (env->buttons & M_D) && ( env->time >= time_old))//control lower menu boundary
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
					if(env->gameState->boughtWeapon & (1<<counter))
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
					if(env->gameState->boughtShip & (1<<counter))
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
		else if(menu_state == SHOP_UPGRADE_WEAPON)//scrollable display with menu cursor
		{
			if(menu_cursor >= 5)
			{
				print("/",13,21);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}

		}

		time_old = env->time + MENU_DELAY;
	}

	if( (env->buttons & M_A) && ( env->time >= time_old))
	{
		displayClear();
		if(menu_state == MAIN)
		{
			if(menu_cursor == 1)//con
			{
				menu_state = CONTINUE3;
				menu_cursor = 2;
			}
			else if(menu_cursor == 2)//inv
			{
				menu_state = INVENTORY;
				menu_cursor = 2;
			}
			else if(menu_cursor == 3)//shop
			{
				menu_state = SHOP;
				menu_cursor = 2;
			}
			else if(menu_cursor == 4)//save
			{
				menu_state = SAVE;
				menu_cursor = 4;
			}
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
				print("/",13,1 + 4 * menu_cursor);
			}
		}
		else if(menu_state == SHOP_WEAPONS)
		{
			if(menu_cursor >= 3){	//menu_cursor = 2 -> GUN UPGRADE
				if((env->gameState->boughtWeapon & (1<<(menu_cursor-3))) == 0)
				{
					if(price_weapons[menu_cursor -3]<= env->gameState->points)
					{
						env->gameState->points = env->gameState->points - price_weapons[menu_cursor-3];
						env->gameState->boughtWeapon |= (1<<(menu_cursor-3));
					}
				}
				else if((env->gameState->boughtWeapon & (1<<(menu_cursor-3))) != 0)
				{
					weapon = menu_cursor -2; //weapon 0 == GUN
					menu_state = SHOP_UPGRADE_WEAPON;
					menu_cursor = 2;
				}
			}
			else if(menu_cursor == 2)
			{
				weapon = menu_cursor -2; //weapon 0 == GUN
				menu_state = SHOP_UPGRADE_WEAPON;
				menu_cursor = 2;
			}

			if(menu_cursor >=5)
			{
				print("/",13,21);
			}
		}
		else if(menu_state == SHOP_SHIPS)
		{
			if(menu_cursor-2 >= 0)
			{
				if((env->gameState->boughtShip & (1<<(menu_cursor-2))) == 0)
				{

					if(price_ships[menu_cursor -2]<= env->gameState->points)
					{
						env->gameState->points = env->gameState->points - price_ships[menu_cursor-2];
						env->gameState->boughtShip |= (1<<(menu_cursor-2));
					}
				}
			}

			if(menu_cursor >=5)
			{
				print("/",13,21);
			}
		}
		//select ships
		else if(menu_state == INVENTORY_SHIPS)
		{
			env->gameState->selShip = menu_cursor -1;
			if(menu_cursor >=5 )
			{
				print("/",13,21);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}
		}
		//select weapons
		else if(menu_state == INVENTORY_WEAPONS)
		{
			env->gameState->selWeapon = menu_cursor -1;
		}
		//upgrade selected weapon
		else if(menu_state == SHOP_UPGRADE_WEAPON)
		{
			//upgrade selected weapon
			//with info from current cursor

		}

		if(menu_state != INVENTORY_SHIPS)
		{
			print("/",13,1 + 4 * menu_cursor);
		}
		time_old = env->time + MENU_DELAY;
	}
	if( (env->buttons & M_B) && ( env->time >= time_old))
	{
		displayClear();
		if(menu_state == INVENTORY)
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
			menu_cursor = weapon + 2;
			if(menu_cursor >= 5)
			{
				print("/",13,21);
				print(" ",13,2);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}

		}
		if(menu_state != SHOP_UPGRADE_WEAPON)
		{
			print("/",13,1 + 4 * menu_cursor);
		}
		time_old = env->time + MENU_DELAY;
	}



	switch(menu_state)
	{
		case MAIN:
			print("CONTINUE",20,5);
			print("INVENTORY",20,9);
			print("SHOP",20,13);
			print("SAVE",20,17);
			break;

		case CONTINUE3:
			displayClear();
			return env->gameState;

		case INVENTORY:
			print("INVENTORY",20,5);
			print("WEAPONS",20,9);
			print("SHIPS",20,13);
			break;
		case INVENTORY_WEAPONS://show all bought weapons
			print("SECUNDARY:",20,4);
			if(env->gameState->selWeapon != 0)
			{
				print(weapons_text[env->gameState->selWeapon],90,4);
			}
			else
			{
				print("NONE",90,4);
			}

			iter =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (env->gameState->boughtWeapon & (1<<iter)))
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(env->gameState->selWeapon >=1 && env->gameState->selWeapon == iter+1)//selWeapon == 0 -> no B weapon slected
						{
							print("current",100,zeile);
						}
						else
						{
							print("       ",100,zeile);
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
					if(zeile <= 21 && (env->gameState->boughtWeapon & (1<<iter)) && iter >= scroll_start)
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(env->gameState->selWeapon >=1 && env->gameState->selWeapon == iter+1)//selWeapon == 0 -> no B weapon slected
						{
							print("current",100,zeile);
						}
						else
						{
							print("       ",100,zeile);
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
			if(env->gameState->selShip != 0)
			{
				print(ships_text[env->gameState->selShip-1],90,4);
			}
			else
			{
				print("NONE",90,4);
			}


			iter =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (env->gameState->boughtShip & (1<<iter)))
					{
						print("          ",20,zeile);
						print(ships_text[iter],20,zeile);
						if(env->gameState->selShip >=1 && env->gameState->selShip == iter+1)//selWeapon == 0 -> no B weapon slected
						{
							print("current",100,zeile);
						}
						else
						{
							print("       ",100,zeile);
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
			printN(env->gameState->points,100,5);

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
						else if((env->gameState->boughtWeapon & (1<<(scroll_start-1))))
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
				while(zeile <= 21 && menu_cursor >= scroll_start)
				{
					print("          ",20,zeile);
					print(weapons_text[scroll_start],20,zeile);

					if(env->gameState->boughtWeapon & (1<<(scroll_start-1)))
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
			printN(env->gameState->points,100,5);

			next_col =0;
			zeile = 9;

			//normal display without scrolling
			if(menu_cursor <= 5)
			{
					while(zeile <= 21 )
					{
						print("          ",20,zeile);
						print(ships_text[next_col],20,zeile);

						if(env->gameState->boughtShip & (1<<next_col))
						{
							print("          ",100,zeile);
							print("BOUGHT",100,zeile);
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
			print("UPGRADE",20,5);
			print("MACHINEGUN",70,5);
			printN(weapon,1,1);
			print("DMG",20,9);
			print("SPEED",20,13);
			print("ROF",20,17);
			print("LVL",20,21);

			break;
		case SAVE:
			safeSave(getCurrentSave(),env);
			menu_state = MAIN;
			menu_cursor = 4;
			break;

	}
}

}



//returns 1 when back to main menu else 0
uint8_t pause_menu(Environment env)
{
	uint32_t time_old = env->time + MENU_DELAY;
	uint8_t menu_state = MAIN;
	uint8_t menu_cursor = 2;
	displayClear();
	print("/",13,1 + 4 * menu_cursor);



	while(1)
	{
		updateEnvironment(env);

		if( (env->buttons & M_U) && ( env->time >= time_old))
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
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_D) && ( env->time >= time_old))
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
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_A) && ( env->time >= time_old))
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
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_B) && ( env->time >= time_old))
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
			time_old = env->time + MENU_DELAY;
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
				displayClear();
				return 0;

			case SAVE2:
				//safe to current safegame and go back to main
				print("SAVE",1,1);
				menu_state = MAIN;
				menu_cursor = 3;
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
				displayClear();
				return 1;

		}
	}
}

//
//void main_menu()
//{
//
//	uint8_t menu_state = MAIN;
//	uint8_t menu_cursor = 2;
//	print("/",13,1 + 4 * menu_cursor);
//	while(1){
//
//		env->time = getMsTimer();
//
//	if( B_UP && ( env->time >= time_old))
//	{
//		print(" ",13,1 + 4 * menu_cursor);
//		menu_cursor-=1;
//
//		if(menu_state == MAIN)
//		{
//			if(menu_cursor <=1)
//			{
//				menu_cursor = 1;
//			}
//		}
//		else if(menu_state == OPTIONS)
//		{
//			if(menu_cursor <= 2)
//			{
//				menu_cursor = 2;
//			}
//		}
//		else
//		{
//			menu_state = MAIN;
//			menu_cursor = 2;
//		}
//
//		print("/",13,1 + 4 * menu_cursor);
//		time_old = getMsTimer() + MENU_DELAY;
//	}
//
//	if( B_DOWN && ( env->time >= time_old))
//	{
//		print(" ",13,1 + 4 * menu_cursor);
//		menu_cursor+=1;
//
//		if(menu_state == MAIN)
//		{
//			if(menu_cursor >= 4)
//			{
//				menu_cursor = 4;
//			}
//
//		}
//		else if(menu_state == OPTIONS)
//		{
//			if(menu_cursor >= 3)
//			{
//				menu_cursor = 3;
//			}
//		}
//		else
//		{
//			menu_cursor = 2;
//			menu_state = MAIN;
//		}
//
//		print("/",13,1 + 4 * menu_cursor);
//		time_old = getMsTimer() + MENU_DELAY;
//	}
//
//	if( B_A && ( env->time >= time_old))
//	{
//		if(menu_state == MAIN)
//		{
//			menu_state = menu_cursor -1;
//			menu_cursor = 2;
//		}
//		else if(menu_state == SELECTGAME)
//		{
//			menu_state = menu_cursor + 20;
//		}
//		else if(menu_state == OPTIONS)
//		{
//			if(menu_cursor == 2) // music
//			{
//				menu_cursor = 2;
//				menu_state = OPTIONS_1;
//			}
//			if(menu_cursor == 3) // invert
//			{
//				menu_state = OPTIONS_2;
//				menu_cursor = 3;
//			}
//		}
//		else if(menu_state == HIGHSCORES)
//		{
//			//do nothing
//			menu_state = HIGHSCORES;
//			menu_cursor = 2;
//
//		}
//		else
//		{
//			menu_state = MAIN;
//			menu_cursor = 2;
//		}
//		displayClear();
//		if(menu_state != HIGHSCORES)
//		{
//			print("/",13,1 + 4 * menu_cursor);
//		}
//		time_old = getMsTimer() + MENU_DELAY;
//	}
//
//	if( B_B && ( env->time >= time_old))
//	{
//		if(menu_state == OPTIONS)
//		{
//			menu_state = MAIN;
//			menu_cursor = 4;
//		}
//		if(menu_state == SELECTGAME)
//		{
//			menu_state = MAIN;
//			menu_cursor = 2;
//		}
//		if(menu_state == HIGHSCORES)
//		{
//			menu_state = MAIN;
//			menu_cursor = 3;
//		}
//		else
//		{
//			menu_state = MAIN;
//			menu_cursor = 2;
//		}
//
//		displayClear();
//		print("/",13,1 + 4 * menu_cursor);
//		time_old = getMsTimer() + MENU_DELAY;
//	}
//
//
//	switch(menu_state)
//	{
//		case MAIN:
//			print("CONTINUE",20,5);
//			print("SELECT GAME",20,9);
//			print("HIGHSCORES",20,13);
//			print("OPTIONS",20,17);
//			break;
//
//		case CONTINUE:
//			//do sth
//			print("Cont",1,1);
//			while(1);
//			break;
//
//		case SELECTGAME:
//			print("SELECT GAME",20,5);
//			print("SAVE1",20,9);
//			print("SAVE2",20,9);
//			print("SAVE3",20,9);
//			break;
//
//		case SELECTGAME_1:
//			print("sAVE1",1,1);
//			while(1);
//			break;
//		case SELECTGAME_2:
//			print("sAVE1",1,1);
//			while(1);
//			break;
//		case SELECTGAME_3:
//			print("sAVE1",1,1);
//			while(1);
//			break;
//
//		case HIGHSCORES:
//			print("HIGHSCORES",20,5);
//			print("SAVE1",20,9);
//			print("SAVE2",20,13);
//			print("SAVE3",20,17);
//			break;
//
//
//		case OPTIONS2:
//			print("OPTIONS",20,5);
//			print("Music",20,9);
//			if( set_options & (1<<0))
//			{
//				print("ON",50,9);
//			}
//			else
//			{
//				print("OFF",50,9);
//			}
//
//			print("Invert Colours",20,13);
//			if( set_options & (1<<1))
//			{
//				print("ON  ",106,13);
//			}
//			else
//			{
//				print("OFF",106,13);
//			}
//			break;
//
//		case OPTIONS_1:
//			if( set_options & (1<<0))
//			{
//				set_options &= ~(1<<0);
//			}
//			else
//			{
//				set_options |= (1<<0);
//			}
//			//disable music
//			//so sth
//			menu_state = OPTIONS2;
//			menu_cursor = 2;
//
//			break;
//
//		case OPTIONS_2:
//			if( set_options & (1<<1))
//			{
//				set_options &= ~(1<<1);
//			}
//			else
//			{
//				set_options |= (1<<1);
//			}
//			displayInverse(set_options & (1<<1));
//			menu_state = OPTIONS2;
//			menu_cursor = 3;
//			break;
//	}
//}
//}
