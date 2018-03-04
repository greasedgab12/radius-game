#ifndef MENU_H
#define MENU_H

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


void display_mainmenu(void);
void display_gamemenu(void);


#endif
