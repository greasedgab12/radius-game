#ifndef MENU_H
#define MENU_H

#include"object.h"

#define MENU_DELAY 5
#define MAIN 0
#define MAIN_UPPER_BOUNDARY 1
#define MAIN_LOWER_BOUNDARY 4
#define CONTINUE3 6
#define INVENTORY 1
#define INVENTORY_OFFSET 10
#define INVENTORY_WEAPONS 11
#define INVENTORY_SHIPS 12
#define IVENTORY_UPPER_BONDARY 2
#define INVENTORY_LOWER_BOUNDARY 3
#define INVENTORY_WEAPONS_UPPER_BOUNDARY 2
#define INVENTORY_SHIPS_UPPER_BOUNDARY 2
#define SHOP 2
#define SHOP_WEAPONS 21
#define SHOP_SHIPS 22
#define SHOP_UPGRADE_WEAPON 23
#define SHOP_UPGRADE_SHIP 24
#define SHOP_UPPER_BOUNDARY 2
#define SHOP_LOWER_BOUNDARY 3
#define SHOP_LOWER_BOUNDARY2 5
#define SHOP_WEAPONS_UPPER_BOUNDARY 2
#define SHOP_SHIPS_UPPER_BOUNDARY 2
#define SHOP_UPGRADE_WEAPON_UPPER_BOUNDARY 2
#define SAVE 3
#define SAVE_UPPER_BOUNDARY 2
#define SAVE_LOWER_BOUNDARY 5
#define CONTINUE 1
#define SAVE2 2
#define OPTIONS 3
#define OPTIONS_1 31
#define OPTIONS_2 32
#define QUIT 4
#define SELECTGAME 2
#define SELECTGAME_1 21
#define SELECTGAME_2 22
#define SELECTGAME_3 23
#define HIGHSCORES 3
#define OPTIONS2 4
#define OPTIONS_UPPER_BOUNDARY 2
#define OPTIONS_LOWER_BOUNDARY 4


void titleScreen();
void displayLevel(Environment env);
void displayStart(Environment env);
void displayFinished(Environment env);

void displayGameOver(Environment env);
GameState weaponUpgrade(GameState gameState,uint8_t weapon,uint8_t menu_cursor);
void printUpgrade(GameState gameState, uint8_t weapon);
void  shop_menu(Environment env);
uint8_t pause_menu(Environment env);
void main_menu(Environment env);

#endif
