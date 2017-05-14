#pragma once
#include "Constants.h"
#include "Includes.h"
#include "Snake.h"

typedef struct Game {
	TCHAR ** map;
	Snake snakes[10];
}Game;