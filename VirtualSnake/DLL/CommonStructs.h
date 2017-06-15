#pragma once
#include <Windows.h>
#include <tchar.h>
#include "CommonConstants.h"

typedef struct PipeMessage {

	TCHAR map[MAP_ROWS][MAP_COLUMNS]; 
	boolean snakeAlive;

}PipeMessage;

typedef struct Highscore {
	TCHAR playerName[TAM];
	int score;

}Highscore;