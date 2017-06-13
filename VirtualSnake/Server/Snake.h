#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"

typedef struct Snake {
	int size;
	int id;
	float speed;
	int score; 
	int *bodyRow;		
	int *bodyColumn;    //1st position -> tail, size position -> head
	TCHAR direction;
	boolean alive;
	boolean isActive;
	boolean isDrunk;
	int effectTimer;
}Snake;
