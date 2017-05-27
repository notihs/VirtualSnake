#pragma once
#include "Includes.h"
#include "..\DLL\dll.h"
#include "..\DLL\CommonConstants.h"

DWORD WINAPI WaitForLocalClients(LPVOID param);
DWORD WINAPI readKeyPressedByClient(LPVOID param);
DWORD WINAPI WaitForRemoteClients(LPVOID param);
void initPipes();
DWORD WINAPI ReadFromClientX(LPVOID param);


