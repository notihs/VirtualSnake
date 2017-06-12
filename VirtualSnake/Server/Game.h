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



void showSnakeInfo();
void initSnakes();
void initGame(); 
void startGame();
void tryToMoveSnake(int, TCHAR);
Game * getCurrentGame();
void standardMovement(int, int, int);
void eatingMovement(int, int, int);
bool validMovement(int, TCHAR);
void eatingOil(int, int, int);
void eatingGlue(int, int, int);
void killSnake(int);
void eatingGrenade(int, int, int);
void opponentGlued(int, int, int);
void opponentOiled(int, int, int);
void hitWall(int);
void drinkVodka(int, int, int);
void opponentDrunk(int, int, int);
TCHAR swapKeys(TCHAR);
void getFrozen(int, int, int); //SO diminiu se a cobra tiver maior tamanho do que o inicial
void nextTile(int, int, int);