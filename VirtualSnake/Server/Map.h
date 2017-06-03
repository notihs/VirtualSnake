#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"
#include "Snake.h"

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



TCHAR ** createMap();
void showMap(TCHAR **);
void generateItems(TCHAR **, int);
void generateSnakes(TCHAR **, int, Snake**);
