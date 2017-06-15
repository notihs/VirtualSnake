#pragma once
#include "Includes.h"
#include "..\DLL\CommonConstants.h"
#include "..\DLL\CommonStructs.h"

void sortByDescendingScore(int, Highscore);
void writeOnRegistry(TCHAR playerName[TAM], int score);