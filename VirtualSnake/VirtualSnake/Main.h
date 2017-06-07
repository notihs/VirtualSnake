#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "..\DLL\CommonConstants.h"
#include "..\DLL\dll.h"
#include "Utils.h"

//void mapInMemory();
void remoteGame();
void localGame(TCHAR **);
DWORD WINAPI WaitForMapChanges(LPVOID param);
TCHAR ** initMalloc();