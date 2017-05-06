#pragma once
#include "Constants.h"
#include "Includes.h"
#include "Snake.h"

typedef struct Jogo {
	TCHAR ** map;
	Snake snakes[10];
};