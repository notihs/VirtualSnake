#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"

typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionX;
	int headPositionY;
	bool alive;
}Snake;

Snake initOneSnake(int);