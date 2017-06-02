#pragma once
#include "..\DLL\CommonConstants.h"
#include "Includes.h"
#include "Snake.h"
#include "Map.h"
#include "..\DLL\dll.h"

/*typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionRow;
	int headPositionColumn;
	int tailPositionRow;
	int tailPositionColumn;
	bool alive;
}Snake;*/

typedef struct Game {
	TCHAR ** map;
	//Snake snake[MAX_PLAYERS];
	Snake ** snake;
	int activePlayers;
	bool hasStarted;
}Game;



Snake initOneSnake(int); //deprecated
void showSnakeInfo();
void initSnakes(Snake **snake, int activePlayers); //deprecated
void initSnakes();
void initGame(); 
void startGame();
void tryToMoveSnake(int player, TCHAR keyPressed);
Game * getCurrentGame();