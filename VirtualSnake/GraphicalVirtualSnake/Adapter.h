#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "..\DLL\CommonConstants.h"
#include "..\DLL\dll.h"
#include "..\DLL\CommonStructs.h"


boolean onCreate();
TCHAR ** initMalloc();
boolean localGame();
DWORD WINAPI WaitForMapChanges(LPVOID param);