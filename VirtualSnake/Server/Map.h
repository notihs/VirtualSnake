#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"
//#include "Game.h"

using namespace std;

#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#define tstring wstring

#else
#define tcout cout
#define tcin cin
#define tstring string
#endif

typedef struct Snake {
	int size;
	int id;
	float speed;
	int headPositionX;
	int headPositionY;
	int tailPositionX;
	int tailPositionY;
	bool alive;
}Snake;

TCHAR ** createMap();
void showMap(TCHAR **);
void generateItems(TCHAR **);
void generateSnakes(TCHAR **, int, Snake*);
