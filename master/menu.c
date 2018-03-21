#include "char.h"
#include <inttypes.h>
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "uart.h"
#include "savegame.h"
#include "environment.h"
#include "object.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include "adc.h"
#include "sprites.h"
#include "sprite.h"

uint8_t set_options 	= 0b00000001;// keep track of selected options (currently only 2 bits in use room for 6 more


const char *weapons_text[] = {"GUN","MACHINEGUN","MULTISHOT","HEAVY","SHOTGUN","NOPPY","LAUNCHER","BOUNCE","LASER"};   //names of the different weapons to make it easier to list them all on the display
const char *ships_text[] = {"PATROL","LIGHT","HEAVY","DESTROYER"};	//names of the different ships to make it easier to list them all on the display

uint8_t price_weapons[] = { 15,115,201,61,32,86,131,210,250 };//weapon price
uint8_t price_ships[] =  { 0,120,140,230,10,10,10 };	//ship price
uint8_t price_upgrade[] = {1,2,3,4};	//price modifier when ugrading a weapon
uint8_t weapon = 0;			//

uint32_t time_old = 0;		//previous Time a button was pressed + DELAY to keep the cursor from crolling to fast
uint8_t menu_state = MAIN;	//keeps track of the menu state to select the correct entries
uint8_t menu_cursor = 2;	//keeps track of the cursor so it can select the correct entries

uint8_t upgrade =0;
uint8_t offset =0;		//offset to select the to be upgraded weapon attribute
uint8_t nextZeile = 9;	//keeps track of the display rows (5->21)


//display gameover screen
void displayGameOver(Environment env){
	sendWindow(20,5,90,16,0);
	print("Game Over", 57, 5);
	_delay_ms(300);
	if(saveHighScore(env->gameState->points)){
		print("NEW HIGHSCORE:",19,9);
		_delay_ms(300);
		printN(env->gameState->points,50,13);
	}
	_delay_ms(3000);
}

//display title screen that was missing in our presentation
void titleScreen(){

	drawTitleScreen();
	_delay_ms(1000);
	displayClear();
}

//display level at start
void displayLevel(Environment env){

	if(env->level < 10)
	{
		print("LEVEL ",60,10);
		printN(env->level,96,10);
	}
	else
	{
		print("LEVEL ",54,10);
		printN(env->level,90,10);
	}
	_delay_ms(2000);

	displayClear();
}
//display start screen
void displayStart(Environment env){
	print("READY",60,10);
	_delay_ms(1000);
	print("START",60,14);
	_delay_ms(1000);
	displayClear();
}

//display end screen
void displayFinished(Environment env){
	if(env->level < 10)
	{
		print("LEVEL ",60,10);
		printN(env->level,96,10);
		print("CLEARED",60,14);
	}
	else
	{
		print("LEVEL ",54,10);
		printN(env->level,90,10);
		print("CLEARED",56,14);
	}

	if(env->gameState->points <10)
	{
		print("POINTS:",54,18);
		printN(env->gameState->points,100,18);
	}
	else if(env->gameState->points <100)
	{
		print("POINTS:",54,18);
		printN(env->gameState->points,96,18);
	}
	else if(env->gameState->points <1000)
	{
		print("POINTS:",48,18);
		printN(env->gameState->points,86,18);
	}
	else
	{
		print("POINTS:",46,18);
		printN(env->gameState->points,86,18);
	}

	_delay_ms(3000);
	displayClear();
}

//upgrade the selected weapon
GameState weaponUpgrade(GameState gameState,uint8_t weapon,uint8_t menu_cursor)
{
	upgrade =0;
	offset =0;

	//select offset per cursor
	if(menu_cursor == 2 )
	{
		offset = 0;
	}
	else if(menu_cursor == 3)
	{
		offset = 2;
	}
	else if(menu_cursor == 4)
	{
		offset = 4;
	}
	else if(menu_cursor == 5)
	{
		offset = 6;
	}

	if(weapon == 0)//gun
	{
		upgrade = gameState->gunUpg;
	}
	else if(weapon == 1)//machine
	{
		upgrade = gameState->machineGunUpg;
	}
	else if(weapon == 2)//multi
	{
		upgrade = gameState->multiUpg;
	}
	else if(weapon == 3)//heavy
	{
		upgrade = gameState->heavyUpg;
	}
	else if(weapon == 4)//shot
	{
		upgrade = gameState->shotGunUpg;
	}
	else if(weapon == 5)//noppy
	{
		upgrade = gameState->noppyUpg;
	}
	else if(weapon == 6)//launcher
	{
		upgrade = gameState->launcherUpg;
	}
	else if(weapon == 7)//bounceUpg
	{
		upgrade = gameState->bounceUpg;
	}
	else if(weapon == 8)//lasorUpg
	{
		upgrade = gameState->laserUpg;
	}


	//upgrade the weapon: 2bit = 1 attribute (1-4 level) that can be upgraded
	if((upgrade & (0b11<<offset)) == (0b00<<offset))
	{
		if(price_upgrade[0]* price_weapons[weapon] <= gameState->points)
		{
			gameState->points = gameState->points - (price_upgrade[0]* price_weapons[weapon]) ;
			upgrade &= ~(0b11<<offset);
			upgrade |= (0b01<<offset);
		}
	}
	else if ((upgrade & (0b11<<offset)) == (0b01<<offset))
	{
		if(price_upgrade[1]* price_weapons[weapon] <= gameState->points)
		{
			gameState->points = gameState->points - (price_upgrade[1]* price_weapons[weapon]) ;
			upgrade &= ~(0b11<<offset);
			upgrade |= (0b10<<offset);
		}
	}
	else if ((upgrade & (0b11<<offset)) == (0b10<<offset))
	{
		if(price_upgrade[2]* price_weapons[weapon] <= gameState->points)
		{
			gameState->points = gameState->points -price_upgrade[0]* price_weapons[weapon] ;
			upgrade &= ~(0b11<<offset);
			upgrade |= (0b11<<offset);
		}
	}

	if(weapon == 0)//gun
	{
		gameState->gunUpg = upgrade;
	}
	else if(weapon == 1)//machine
	{
		gameState->machineGunUpg = upgrade;
	}
	else if(weapon == 2)//multi
	{
		gameState->multiUpg = upgrade;
	}
	else if(weapon == 3)//heavyUpg
	{
		gameState->heavyUpg = upgrade;
	}
	else if(weapon == 4)//shotGunUpg
	{
		gameState->shotGunUpg = upgrade;
	}
	else if(weapon == 5)//noppyUpg
	{
		gameState->noppyUpg = upgrade;
	}
	else if(weapon == 6)//launcherUpg
	{
		gameState->launcherUpg = upgrade;
	}
	else if(weapon == 7)//bounceUpg
	{
		gameState->bounceUpg = upgrade;
	}
	else if(weapon == 8)//bounceUpg
	{
		gameState->laserUpg = upgrade;
	}

	return gameState;
}

//display the upgrade screen with the current stats
void printUpgrade(GameState gameState, uint8_t weapon)
{
	offset = 0;
	upgrade=0;
	nextZeile = 9;

	if(weapon == 0)//gun
	{
		upgrade = gameState->gunUpg;
	}
	if(weapon == 1)//machineGunUpg
	{
		upgrade = gameState->machineGunUpg;
	}
	if(weapon == 2)//multiUpg
	{
		upgrade = gameState->multiUpg;
	}
	if(weapon == 3)//heavyUpg
	{
		upgrade = gameState->heavyUpg;
	}
	if(weapon == 4)//shotGunUpg
	{
		upgrade = gameState->shotGunUpg;
	}
	if(weapon == 5)//noppyUpg
	{
		upgrade = gameState->noppyUpg;
	}
	if(weapon == 6)//launcherUpg
	{
		upgrade = gameState->launcherUpg;
	}
	if(weapon == 7)//bounceUpg
	{
		upgrade = gameState->bounceUpg;
	}
	if(weapon == 8)//laserUpg
	{
		upgrade = gameState->laserUpg;
	}
	print("UPGRADE",20,2);
	print(weapons_text[weapon],24,5);
	print32(gameState->points,110,5);
	print("$",100,5);

	print("DMG",20,9);
	print("SPEED",20,13);
	print("ROF",20,17);
	print("LVL",20,21);

	print("      ",90,25);
	while(offset<=6)
	{
		if((upgrade & (0b11<<offset)) == (0b00<<offset))
		{
			print("%",60,nextZeile);
			printN(price_upgrade[0]* price_weapons[weapon],110,nextZeile);
			print("$",100,nextZeile);
		}
		else if ((upgrade & (0b11<<offset)) == (0b01<<offset))
		{
			print("%%",60,nextZeile);
			printN(price_upgrade[1]* price_weapons[weapon],110,nextZeile);
			print("$",100,nextZeile);
		}
		else if ((upgrade & (0b11<<offset)) == (0b10<<offset))
		{
			print("%%%",60,nextZeile);
			printN(price_upgrade[2]* price_weapons[weapon],110,nextZeile);
			print("$",100,nextZeile);
		}
		else if ((upgrade & (0b11<<offset)) == (0b11<<offset))
		{
			print("%%%%",60,nextZeile);
			print("-",110,nextZeile);
			print("$",100,nextZeile);
		}
		offset += 2;
		nextZeile += 4;
		print("      ",90,25);
	}
}

//choose/buy weapons/ships from inventory or upgrade them
void shop_menu(Environment env)
{
	displayClear();

	uint8_t iter =0;
	uint8_t zeile = 9;
	uint8_t next_col = 0;
	uint8_t scroll_start=0;
	uint8_t counter = 0;
	uint8_t lower_boundary = 0;

	uint32_t time_old = env->time + MENU_DELAY;

	print("/",13,1 + 4 * menu_cursor);

	while(1){

	updateEnvironment(env);


	if( (env->buttons & M_U) && ( env->time >= time_old))//keep cursor from scrolling to far up
	{
		uart_putc('k');
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor-=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor <= 2)
			{
				menu_cursor = 2;
			}
		}
		else if(menu_state == INVENTORY)
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
		else if(menu_state == SHOP_UPGRADE_WEAPON)
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

		if(menu_cursor >= 5)
		{
			print("/",13,21);
		}
		else
		{
			print("/",13,1 + 4 * menu_cursor);
		}

		time_old = env->time + MENU_DELAY;
	}

	if( (env->buttons & M_D) && ( env->time >= time_old))//keep cursor from scrolling to far down
	{
		uart_putc('k');
		print(" ",13,1 + 4 * menu_cursor);
		menu_cursor+=1;

		if(menu_state == MAIN)
		{
			if(menu_cursor >= 5)
			{
				menu_cursor = 5;
			}
			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == INVENTORY)
		{
			if(menu_cursor >= INVENTORY_LOWER_BOUNDARY)
			{
				menu_cursor = INVENTORY_LOWER_BOUNDARY;
			}
			print("/",13,1 + 4 * menu_cursor);
		}
		else if(menu_state == INVENTORY_WEAPONS)//lower boundary not clear in inventory -> count bought items to display them correctly
		{
			counter = 0;
			lower_boundary = 2;

			while(counter < 8)
			{
				if(env->gameState->boughtWeapon & (1<<counter))
				{
					lower_boundary++;
				}
				counter++;
			}
			lower_boundary--;

			if(lower_boundary >= 9)
			{
				lower_boundary = 9;
			}

			if(menu_cursor >= lower_boundary )
			{
					menu_cursor = lower_boundary;
			}

			if(menu_cursor <= 2)
			{
					menu_cursor = 2;
			}

			if(menu_cursor <=5)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 6 )
			{
				print("/",13,21);
			}
		}
		else if(menu_state == INVENTORY_SHIPS)//lower boundary not clear in inventory -> count bought items to display them correctly
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
			else if(menu_cursor >= 5 )
			{
				menu_cursor = 5;
				print("/",13,21);
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
		else if(menu_state == SHOP_WEAPONS)
		{
			if(menu_cursor <=4)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 5)
			{
				print("/",13,21);
			}

			if(menu_cursor >=10)
			{
				menu_cursor = 10;
				print("/",13,21);
			}
		}
		else if(menu_state == SHOP_SHIPS)
		{
			if(menu_cursor <=5)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			else if(menu_cursor >= 6 )
			{
				menu_cursor = 6;
				print("/",13,21);
			}
		}
		else if(menu_state == SHOP_UPGRADE_WEAPON)
		{
			if(menu_cursor >= 5)
			{
				menu_cursor = 5;
				print("/",13,21);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}
		}

		time_old = env->time + MENU_DELAY;
	}

	if( (env->buttons & M_A) && ( env->time >= time_old))	//define action when button a is pressed
	{
		uart_putc('k');
		displayClear();
		if(menu_state == MAIN)
		{
			if(menu_cursor == 2)//con
			{
				menu_state = CONTINUE3;
				menu_cursor = 2;
			}
			else if(menu_cursor == 3)//inv
			{
				menu_state = INVENTORY;
				menu_cursor = 2;
			}
			else if(menu_cursor == 4)//shop
			{
				menu_state = SHOP;
				menu_cursor = 2;
			}
			else if(menu_cursor == 5)//save
			{
				menu_state = SAVE;
				menu_cursor = 5;
			}
		}
		else if(menu_state == SHOP)
		{

			if(menu_cursor == 2)
			{
				menu_cursor = 2;
				print("/",13,1+4*menu_cursor);
				menu_state = SHOP_WEAPONS;
			}
			else if(menu_cursor == 3)
			{

				menu_state = SHOP_SHIPS;
				menu_cursor = 2;
				print("/",13,1+4*menu_cursor);
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

			if(menu_cursor >= 3){
				if((env->gameState->boughtWeapon & (1<<(menu_cursor-3))) == 0)
				{
					if(price_weapons[menu_cursor -3]<= env->gameState->points)
					{
						env->gameState->points = env->gameState->points - price_weapons[menu_cursor-3];
						env->gameState->boughtWeapon |= (1<<(menu_cursor-3));
						env->gameState->selWeapon = menu_cursor -2;
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
			else
			{
				print("/",13,1+4*menu_cursor);
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
						env->gameState->selShip = menu_cursor -1;
					}
				}
			}
			if(menu_cursor >=5)
			{
				print("/",13,21);
			}
		}

		else if(menu_state == INVENTORY_SHIPS)
		{
			iter = 0;
			uint8_t cursor_ersatz = menu_cursor;
			while(iter < 5)
			{
				if(env->gameState->boughtShip & (1<<iter))
				{
					cursor_ersatz -= 1;
				}
				if(cursor_ersatz <= 1)
				{
					env->gameState->selShip = iter+1;
					break;
				}
				iter++;
			}
			if(menu_cursor >=5 )
			{
				print("/",13,21);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}
		}

		else if(menu_state == INVENTORY_WEAPONS)
		{
			iter = 0;
			uint8_t cursor_ersatz = menu_cursor;
			while(iter < 8)
			{
				if(env->gameState->boughtWeapon & (1<<iter))
				{
					cursor_ersatz -= 1;
				}
				if(cursor_ersatz <= 1)
				{
					env->gameState->selWeapon = iter+1;
					break;
				}
				iter++;
			}

			print(" ",20,1);
			if(menu_cursor >=5 )
			{
				print("/",13,21);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			print(" ",20,1);
		}

		else if(menu_state == SHOP_UPGRADE_WEAPON)
		{
			env->gameState = weaponUpgrade(env->gameState,weapon,menu_cursor);

			menu_state = SHOP_UPGRADE_WEAPON;

			if(menu_cursor >=5)
			{
				print("/",13,21);
				print(" ",13,9);
			}
			else
			{
				print("/",13,1+4*menu_cursor);
			}
		}

		if(menu_state == SHOP_WEAPONS && menu_cursor >=5)
		{
			print(" ",13,5);
		}

		if(menu_state != INVENTORY_SHIPS && menu_state != SHOP_WEAPONS && menu_state != SHOP_UPGRADE_WEAPON )
		{
			print("/",13,1 + 4 * menu_cursor);
		}
		print(" ",13,6);
		time_old = env->time + MENU_DELAY;
	}

	if( (env->buttons & M_B) && ( env->time >= time_old))//define actions for when button b is pressed
	{
		uart_putc('k');
		displayClear();
		if(menu_state == INVENTORY)
		{
			menu_state = MAIN;
			menu_cursor = INVENTORY + 2;
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
			menu_cursor = SHOP +2;
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
				print(" ",13,9);
				print(" ",13,2);
			}
			else
			{
				print("/",13,1 + 4 * menu_cursor);
			}

		}

		if(menu_state != SHOP_UPGRADE_WEAPON && menu_state !=SHOP_WEAPONS )
		{
			print("/",13,1 + 4 * menu_cursor);
		}
		time_old = env->time + MENU_DELAY;
	}

	print("  ",13,1);

	switch(menu_state)//display menu text
	{
		case MAIN:
			print("SHOP",66,3);
			print("CONTINUE",20,9);
			print("INVENTORY",20,13);
			print("SHOP",20,17);
			print("SAVE",20,21);
			break;

		case CONTINUE3:
			displayClear();
			return;

		case INVENTORY:
			print("INVENTORY",20,5);
			print("WEAPONS",20,9);
			print("SHIPS",20,13);
			break;
		case INVENTORY_WEAPONS:
			print("SECUNDARY:",20,4);

			if(env->gameState->selWeapon == 0)
			{
				print("NONE",90,4);
			}
			else
			{
				print(weapons_text[env->gameState->selWeapon],90,4);
			}

			iter =0;
			zeile = 9;

			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (env->gameState->boughtWeapon & (1<<iter)))
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(env->gameState->selWeapon >=1 && env->gameState->selWeapon == iter+1)
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
			else if(menu_cursor >= 6)
			{
				scroll_start = menu_cursor -5;
				while(iter<=7 )
				{
					if(zeile <= 21 && (env->gameState->boughtWeapon & (1<<iter)) && iter >= scroll_start)
					{
						print("          ",20,zeile);
						print(weapons_text[1+iter],20,zeile);
						if(env->gameState->selWeapon >=1 && env->gameState->selWeapon == iter+1)
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

			if(menu_cursor <= 5)
			{
				while(iter<=7)
				{
					if(zeile <= 21 && (env->gameState->boughtShip & (1<<iter)))
					{
						print("          ",20,zeile);
						print(ships_text[iter],20,zeile);
						if(env->gameState->selShip >=1 && env->gameState->selShip == iter+1)
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
			print("SHOP",60,3);
			print("BUY WEAPONS",20,9);
			print("BUY SHIPS",20,13);
			break;

		case SHOP_WEAPONS:
			print("WEAPONS",20,5);
			print32(env->gameState->points,100,5);
			print("$",90,5);

			next_col =0;
			zeile = 9;
			scroll_start = 0;


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
							print(" ",90,zeile);
						}
						else if((env->gameState->boughtWeapon & (1<<(scroll_start-1))))
						{
							print("       ",100,zeile);
							print("UPGRADE",100,zeile);
							print(" ",90,zeile);
						}
						else
						{
							print("       ",100,zeile);
							printN(price_weapons[scroll_start-1],100,zeile);
							print("$",90,zeile);
						}

						zeile +=4;
						scroll_start++;
					}
			}
			else if(menu_cursor >= 6)
			{
				scroll_start = menu_cursor - 5;

				while(zeile <= 21 && menu_cursor >= scroll_start)
				{
					print("          ",20,zeile);
					print(weapons_text[scroll_start],20,zeile);

					if(env->gameState->boughtWeapon & (1<<(scroll_start-1)))
					{
						print("       ",100,zeile);
						print("UPGRADE",100,zeile);
						print(" ",90,zeile);
					}
					else
					{
						print("       ",100,zeile);
						printN(price_weapons[scroll_start-1],100,zeile);
						print("$",90,zeile);
					}
					zeile +=4;
					scroll_start++;
				}
			}

			if(zeile == 9)
			{
				print("empty",20,9);
				print(" ",13,9);
			}
			break;
		case SHOP_SHIPS:
			print("SHIPS",20,5);
			print32(env->gameState->points,100,5);
			print("$",90,5);

			next_col =0;
			zeile = 9;

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
							print("$",90,zeile);
						}
						zeile +=4;
						next_col ++;
					}
			}
			else if(menu_cursor >= 5)
			{
				menu_cursor = 5;
			}

			break;
		case SHOP_UPGRADE_WEAPON:
			printUpgrade(env->gameState,weapon);


			if(menu_cursor >=5)
			{
				print("/",13,21);
				print(" ",13,9);
			}
			else
			{
				print("/",13,1+4*menu_cursor);

			}

			break;
		case SAVE:
			safeSave(env->gameState);
			print("SAVED",50,21);
			menu_state = MAIN;
			menu_cursor = 5;
			break;
		}
	}
}

//display the pause menu
uint8_t pause_menu(Environment env)
{
	time_old = env->time + MENU_DELAY;
	menu_state = MAIN;
	menu_cursor = 2;
	displayClear();
	print("/",13,1 + 4 * menu_cursor);

	while(1)
	{
		updateEnvironment(env);

		if( (env->buttons & M_U) && ( env->time >= time_old))
		{
			uart_putc('k');
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
			uart_putc('k');
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
			uart_putc('k');
			if(menu_state == MAIN)
			{
				menu_state = menu_cursor -1;
				if(menu_state == SAVE2){
					menu_cursor = 3;
				}
				else menu_cursor = 2;
			}
			else if(menu_state == OPTIONS)
			{
				if(menu_cursor == 2)
				{
					menu_cursor = 2;
					menu_state = OPTIONS_1;
				}
				if(menu_cursor == 3)
				{
					menu_state = OPTIONS_2;
					menu_cursor = 3;
				}
			}
			else if(menu_state == SAVE2)
			{
				menu_state = MAIN;
				menu_cursor = 3;
			}
			displayClear();
			print("/",13,1 + 4 * menu_cursor);
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_B) && ( env->time >= time_old))
		{
			uart_putc('k');
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
				print("PAUSE",65,3);
				print("Continue",20,9);
				print("SAVE GAME",20,13);
				print("OPTIONS",20,17);
				print("QUIT",20,21);
				break;

			case CONTINUE:
				displayClear();
				return 1;

			case SAVE2:
				safeSave(env->gameState);
				print("SAVED",80,13);
				menu_state = MAIN;
				menu_cursor = 3;
				break;

			case OPTIONS:
				print("OPTIONS",20,5);
				print("Tone",20,9);
				if( set_options & (1<<0))
				{
					print("ON",50,9);
					uart_putc('y');
				}
				else
				{
					print("OFF",50,9);
					uart_putc('z');
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
				return 0;
		}
	}
}


void main_menu(Environment env)
{
	menu_state = MAIN;
	menu_cursor = 2;
	displayClear();
	print("/",13,1 + 4 * menu_cursor);

	time_old = env->time + MENU_DELAY;

	while(1)
	{
		updateEnvironment(env);

		if((env->buttons & M_U) && ( env->time >= time_old))
		{
			uart_putc('k');
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor-=1;

			if(menu_state == MAIN)
			{
				if(menu_cursor <=2)
				{
					menu_cursor = 2;
				}
			}
			else if(menu_state == OPTIONS2)
			{
				if(menu_cursor <= 2)
				{
					menu_cursor = 2;
				}
			}

			if(menu_state != HIGHSCORES){
				print("/",13,1 + 4 * menu_cursor);
			}
			time_old = env->time + MENU_DELAY;
		}

		if((env->buttons & M_D) && ( env->time >= time_old))
		{
			uart_putc('k');
			print(" ",13,1 + 4 * menu_cursor);
			menu_cursor+=1;

			if(menu_state == MAIN)
			{
				if(menu_cursor >= 5)
				{
					menu_cursor = 5;
				}

			}
			else if(menu_state == OPTIONS2)
			{
				if(menu_cursor >= 3)
				{
					menu_cursor = 3;
				}
			}

			if(menu_state != HIGHSCORES)
			{
				print("/",13,1 + 4 * menu_cursor);
			}
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_A) && ( env->time >= time_old))
		{
			uart_putc('k');
			if(menu_state == MAIN)
			{
				if(menu_cursor == 2)
				{
					menu_state = CONTINUE;
				}
				else if(menu_cursor == 3)
				{
					menu_state = SELECTGAME;
					menu_cursor = 2;
				}
				else if(menu_cursor == 4)
				{
					menu_state = HIGHSCORES;

				}
				else if(menu_cursor == 5)
				{
					menu_state = OPTIONS2;
					menu_cursor = 2;
				}
			}

			else if(menu_state == SELECTGAME)
			{
				menu_state = menu_cursor + 20;
			}
			else if(menu_state == OPTIONS2)
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
			time_old = env->time + MENU_DELAY;
		}

		if( (env->buttons & M_B) && ( env->time >= time_old))
		{
			uart_putc('k');
			if(menu_state == OPTIONS2)
			{
				menu_state = MAIN;
				menu_cursor = 5;
			}
			else if(menu_state == HIGHSCORES)
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
				print("MAIN MENU",55,3);

				print("CONTINUE",20,9);
				print("NEW GAME",20,13);
				print("HIGHSCORE",20,17);
				print("OPTIONS",20,21);
				break;

			case CONTINUE:
				loadSave(env->gameState);
				return;

			case SELECTGAME:
				newGame(env->gameState);
				return;

			case HIGHSCORES:
				print("C//define actions for when button b is pressedURRENT HIGHSCORE:",20,5);
				print32(loadHighScore(),20,11);
				break;


			case OPTIONS2:
				print("OPTIONS",60,3);
				print("Tone",20,9);
				if( set_options & (1<<0))
				{
					print("ON",50,9);
					uart_putc('y');
				}
				else
				{
					print("OFF",50,9);
					uart_putc('z');
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









