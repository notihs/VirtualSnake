#pragma once
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>


using namespace std;

#define WIDTH 5
#define HEIGHT 5


#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#define tstring wstring

#else
#define tcout cout
#define tcin cin
#define tstring string
#endif

/*TCHAR ** createMap();
void showMap(TCHAR **);
void generateItems(TCHAR **map);*/

void generateItems(TCHAR map[][HEIGHT]);
void createMap();
void showMap(TCHAR p[][HEIGHT]);

