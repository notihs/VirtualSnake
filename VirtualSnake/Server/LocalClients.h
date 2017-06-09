#pragma once
#include "Includes.h"
#include "..\DLL\dll.h"
#include "..\DLL\CommonConstants.h"
#include "Game.h"

DWORD WINAPI waitForLocalClients(LPVOID param);
DWORD WINAPI moveSnakeToDirectionLocal(LPVOID param);
DWORD WINAPI readKeyPressedByLocalClient(LPVOID param);



