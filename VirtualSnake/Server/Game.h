#pragma once
#include "..\DLL\CommonConstants.h"
#include "Includes.h"
#include "Snake.h"

typedef struct Game {
	TCHAR ** map;
	Snake snake[MAX_PLAYERS];
}Game;