#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"

typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionRow;
	int headPositionColumn;
	int tailPositionRow;
	int tailPositionColumn;
	bool alive;
}Snake;
