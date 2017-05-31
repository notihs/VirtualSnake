#pragma once
#include "..\DLL\CommonConstants.h"
#include "Includes.h"
//#include "Snake.h"
#include "Map.h"
#include "..\DLL\dll.h"

/*typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionX;
	int headPositionY;
	int tailPositionX;
	int tailPositionY;
	bool alive;
}Snake;*/

typedef struct Game {
	TCHAR ** map;
	//Snake snake[MAX_PLAYERS];
	Snake * snake;
	int activePlayers;
	bool hasStarted;
}Game;



Snake initOneSnake(int); //deprecated
void showSnakeInfo(Game *game);
void initSnakes(Snake *snake, int activePlayers);
void initGame(Game *game); 
void startGame(Game *game);
void tryToMoveSnake(Game * game, int player, TCHAR keyPressed);