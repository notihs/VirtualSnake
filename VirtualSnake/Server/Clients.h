#pragma once
#include "Includes.h"
#include "..\DLL\dll.h"
#include "..\DLL\CommonConstants.h"
#include "Game.h"

DWORD WINAPI WaitForLocalClients(LPVOID param);
DWORD WINAPI moveSnakeToDirection(LPVOID param);
DWORD WINAPI readKeyPressedByLocalClient(LPVOID param);
DWORD WINAPI WaitForRemoteClients(LPVOID param);
void initPipes();
DWORD WINAPI ReadFromClientX(LPVOID param);


