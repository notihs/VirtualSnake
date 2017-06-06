#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"

typedef struct Snake {
	int size;
	int id;
	float speed;

	int *bodyRow;		
	int *bodyColumn;    //1st position -> tail, size position -> head
	/*int headPositionRow;
	int headPositionColumn;
	int tailPositionRow;
	int tailPositionColumn;*/
	bool alive;
}Snake;
