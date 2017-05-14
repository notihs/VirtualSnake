#pragma once
#include "Includes.h"

typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionX;
	int headPositionY;
	bool alive;
}Snake;