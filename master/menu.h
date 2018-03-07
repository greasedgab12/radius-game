#ifndef MENU_H
#define MENU_H

#include"object.h"
//	MENU LAYOUT
//  PLAY
//	HIGHSCORES
//  OPTIONS
//	MUSIC

//---PLAY---------------------
//CONTINUE -game info-  		--continue from last saved save
//SELECT SAVEGAME				--menu to select other savegame to play than continuing with the last
//NEW GAME						--select savegame that gets reset for a new playsession

//------CONTINUE

//------SELECT SAVEGAME
//SAVE1
//SAVE2
//SAVE3

//------NEW GAME
//SAVE1
//SAVE2
//SAVE3
//----------------------------


//---HIGHSCORES---------------
//display table where safegames are sorted by points/achievments
//----------------------------

//---OPTIONS------------------
//MUSIC  ON/OFF
//INVERT COLOURS
//RESET ALL SAVEGAMES
//----------------------------

//---MUSIC--------------------
//
//----------------------------

#define MEN_MAIN 0
#define MEN_PLAY 1
#define MEN_CONTINUE 11
#define MEN_SAVEGAME 12
#define MEN_SAVEGAME_FUNC 14
#define MEN_NEWGAME 13
#define MEN_NEWGAME_FUNC 15

#define MEN_HIGHSCORES 2

#define MEN_OPTIONS 3
#define MEN_OPT1 31
#define MEN_OPT2 32
#define MEN_OPT3 33
#define MEN_OPT3_DEL 34

#define MEN_MUSIC 4
#define MEN_MUSIC_PLAY 41

#define SIZE_MEN_MAIN 4
#define SIZE_MEN_PLAY 3
#define SIZE_MEN_SAVEGAME 5
#define SIZE_MEN_NEWGAME 5
#define SIZE_MEN_HIGHSCORES 5
#define SIZE_MEN_OPTIONS 3
#define SIZE_MEN_OPT3 4
#define SIZE_MEN_MUSIC 5
#define MEN_EXIT 99
#define MENU_DELAY 225



#define GAME_MAIN 0
#define GAME_CONTINUE 1

#define GAME_SAVE     2

#define GAME_SHOP 3
#define GAME_SHOP_WEAPONS 32
#define GAME_SHOP_SHIPS   33
#define GAME_SHOP_SELECT  31
#define GAME_SHOP_SELECT_WEAPON 34
#define GAME_SHOP_SELECT_SHIP 35

#define GAME_OPTIONS    4
#define GAME_OPTIONS_1  41
#define GAME_OPTIONS_2  42




#define SIZE_GAME_MAIN 4
#define SIZE_GAME_CONTINUE 1
#define SIZE_GAME_SHOP 4
#define SIZE_GAME_SHOP_WEAPONS 5
#define SIZE_GAME_SHOP_SHIPS 4
#define SIZE_GAME_SHOP_SELECT 3
#define SIZE_GAME_OPTIONS 2
#define SIZE_GAME_SAVE 1
#define SIZE_GAME_QUIT 1

#define PRICE_WEAPON_1 99
#define PRICE_WEAPON_2 110
#define PRICE_WEAPON_3 130
#define PRICE_WEAPON_4 180

#define PRICE_SHIP_1 80
#define PRICE_SHIP_2 140
#define PRICE_SHIP_3 230
#define PRICE_SHIP_4

void display_mainmenu(void);
Object display_gamemenu(Object Player);


#endif
