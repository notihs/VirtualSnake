#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"

typedef struct Snake {
	int size;
	int id;
	float speed;
	//int score; -> TODO: pontuacao (implementar)
	int *bodyRow;		
	int *bodyColumn;    //1st position -> tail, size position -> head
	TCHAR direction;
	bool alive;
}Snake;
