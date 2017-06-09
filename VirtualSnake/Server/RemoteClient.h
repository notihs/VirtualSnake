#pragma once
#include "Includes.h"
#include "..\DLL\dll.h"
#include "..\DLL\CommonConstants.h"
#include "Game.h"


DWORD WINAPI waitForRemoteClients(LPVOID param);
void initPipes();
DWORD WINAPI readKeyFromRemoteClient(LPVOID param);
DWORD WINAPI moveSnakeToDirectionRemote(LPVOID param);
void Security(SECURITY_ATTRIBUTES &sa);
void Cleanup(PSID pEveryoneSID, PSID pAdminSID, PACL pACL, PSECURITY_DESCRIPTOR pSD);