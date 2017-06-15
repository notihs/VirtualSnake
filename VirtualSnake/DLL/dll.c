//DLL.cpp
#include <windows.h>
#include <stdio.h>
//#include "DLL.h"

#include "..\..\VirtualSnake\DLL\dll.h"
#include "CommonStructs.h"
//Para verificar ao carregar a dll que a aplicação irá ocupar mais memória
char ponteiro[40960];

//------------------------------------------------------


//EVENTOS
TCHAR eventKeyPressedName[TAM];
HANDLE hEventKeyPressed[MAX_PLAYERS];

TCHAR eventNewClientName[TAM] = TEXT("Event new client");
HANDLE hEventNewClient;

TCHAR eventGameStartedName[TAM] = TEXT("Event game started");
HANDLE hEventGameStarted;

TCHAR eventMapChangesName[TAM];// = TEXT("Event map changes");
HANDLE hEventMapChanges[MAX_PLAYERS];

TCHAR eventServerIsOnlineName[TAM] = TEXT("Event map changes");
HANDLE hEventServerIsOnline;

TCHAR eventSnakeDiedName[TAM];// = TEXT("Event snake died");
HANDLE hEventSnakeDied[MAX_PLAYERS];

//MUTEX
TCHAR mutexWritingKeyName[TAM];
HANDLE hMutexWritingKey[MAX_PLAYERS];

//MAPA
HANDLE hMapMemory;
TCHAR *mapInMemoryName = TEXT("MapMemory");
TCHAR(*ptrMapInMemory)[MAP_ROWS][MAP_COLUMNS];

//TECLAS
int ownId = -1;
HANDLE hKeysMemory;
TCHAR *keysInMemoryName = TEXT("KeysMemory");
TCHAR *ptrKeysInMemory; //LIST OF KEYS:
									  //e -> empty
									  //o -> occupied
									  //u -> up
									  //d -> down
									  //l -> left
									  //r -> right
									  //- -> dead

//NOMES DOS JOGADORES
HANDLE hUsernameMemory;
TCHAR *usernameMemoryName = TEXT("UsernameMemory");
TCHAR (*ptrUsernameInMemory)[MAX_PLAYERS][TAM];


void initSynchHandles() {

	for (int i = 0; i < MAX_PLAYERS; i++) {
		_stprintf_s(eventKeyPressedName, TAM, TEXT("Key Pressed Event %d"), i);
		_stprintf_s(mutexWritingKeyName, TAM, TEXT("Key Writing Mutex %d"), i);
		_stprintf_s(eventMapChangesName, TAM, TEXT("Event map changes %d"), i);
		_stprintf_s(eventSnakeDiedName, TAM, TEXT("Event snake died %d"), i);

		hEventKeyPressed[i] = CreateEvent(NULL, TRUE, FALSE, eventKeyPressedName);
		hEventMapChanges[i] = CreateEvent(NULL, TRUE, FALSE, eventMapChangesName);
		hEventSnakeDied[i] = CreateEvent(NULL, TRUE, FALSE, eventSnakeDiedName);
		
		hMutexWritingKey[i] = CreateMutex(NULL, FALSE, mutexWritingKeyName);
		
		if (hEventKeyPressed[i] == NULL || hEventMapChanges[i] == NULL || hEventSnakeDied[i] == NULL){
			_tprintf(TEXT("[Erro]Criação de eventos(%d)\n"), GetLastError());
			return;
		}
		else if (hMutexWritingKey[i] == NULL) {
			_tprintf(TEXT("[Erro]Criação de mutexes(%d)\n"), GetLastError());
			return;
		}

	}

	hEventServerIsOnline = CreateEvent(NULL, TRUE, FALSE, eventServerIsOnlineName);

	hEventGameStarted = CreateEvent(NULL, TRUE, FALSE, eventGameStartedName);

	hEventNewClient = CreateEvent(NULL, TRUE, FALSE, eventNewClientName); //Evento criado a FALSE 
																		  //Set -> mete a TRUE
																		  //Reset -> mete a FALSE

	if (hEventNewClient == NULL || hEventGameStarted == NULL || hEventServerIsOnline == NULL) {
		_tprintf(TEXT("[Erro]Criação de evento(%d)\n"), GetLastError());
		return;
	}
}

void initMemory() {

	hKeysMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_PLAYERS]), keysInMemoryName);

	hMapMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), mapInMemoryName);

	hUsernameMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_PLAYERS][TAM]), usernameMemoryName);


	if (hKeysMemory == NULL || hMapMemory == NULL || hUsernameMemory == NULL ){
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return;
	}

	ptrKeysInMemory = (TCHAR(*))MapViewOfFile(hKeysMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR) * MAX_PLAYERS);

	ptrMapInMemory = (TCHAR(*)[MAP_ROWS][MAP_COLUMNS])MapViewOfFile(hMapMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]));

	ptrUsernameInMemory = (TCHAR(*)[MAX_PLAYERS][TAM])MapViewOfFile(hUsernameMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_PLAYERS][TAM]));

	if (ptrKeysInMemory == NULL || ptrMapInMemory == NULL || ptrUsernameInMemory == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
		return;
	}

}

void setOwnId(int id) {
	ownId = id;
}

void initArrayOfKeys() {

	for (int i = 0; i < MAX_PLAYERS; i++) { 
		ptrKeysInMemory[i] = 'e';
		//tcout << TEXT("Posicao ") << i << TEXT(": ") << ptrKeysInMemory[i] << endl;
		//_tprintf(TEXT("Posicao %d: %c\n"), i, ptrKeysInMemory[i]);
	}

	return;
}

void readKeys() { 

	for (int i = 0; i < MAX_PLAYERS; i++) {
		//(*ptrKeysInMemory)[i] = 'e';
		//tcout << TEXT("Posicao ") << i << TEXT(": ") << ptrKeysInMemory[i] << endl;
	}

}

void newKeyPressed(TCHAR tecla) { 
	WaitForSingleObject(hMutexWritingKey[ownId], INFINITE);

	ptrKeysInMemory[ownId] = tecla;

	ReleaseMutex(hMutexWritingKey[ownId]); 
	SetEvent(hEventKeyPressed[ownId]); // Mete evento a TRUE

}

//TODO: carefull here!
void setUsername(TCHAR username[TAM]) {

	_tcscpy_s((*ptrUsernameInMemory)[ownId], TAM, username);
	
	_tprintf(TEXT("%s"), (*ptrUsernameInMemory)[ownId]);
}

int getOwnKeyArrayPosition() {

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (ptrKeysInMemory[i] == 'e') {

			WaitForSingleObject(hMutexWritingKey[i], INFINITE);

			ptrKeysInMemory[i] = 'o'; 
			
			ReleaseMutex(hMutexWritingKey[i]);

			ownId = i;
			SetEvent(hEventNewClient);
			return i;
		}
		//tcout << TEXT("Posicao ") << i << TEXT(": ") << (*ptrKeysInMemory)[i] << endl;
	}

	return -1; //Nao ha posicoes livres
}

void writeMapInMemory(TCHAR ** map) {

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			(*ptrMapInMemory)[i][j] = map[i][j];
			//tcout << (*ptrMapInMemory)[i][j];
		}
		//tcout << endl;
	}

	for (int i = 0; i < MAX_PLAYERS;i++) {
		SetEvent(hEventMapChanges[i]);
	}
	
	return;
}

boolean readMapInMemory(TCHAR ** map) {

	
	if (ptrKeysInMemory[ownId] == '-') {
		return FALSE;
	}


	WaitForSingleObject(hEventMapChanges[ownId], INFINITE);
		
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			map[i][j] = (*ptrMapInMemory)[i][j];
			
		}
		
	}

	ResetEvent(hEventMapChanges[ownId]);

	return TRUE;
	//}
	//return false;
}

//TODO: close all handles here!
void destroyMap() {
	UnmapViewOfFile(ptrMapInMemory);
	UnmapViewOfFile(ptrKeysInMemory);
	CloseHandle(hMapMemory);
	CloseHandle(hKeysMemory);
}

void readAllKeysOnRegistry() {
	Highscore highscore;
	HKEY registryKey;
	DWORD result;
	DWORD sizeOfStruct;
	DWORD dwType = REG_BINARY;
	TCHAR keyName[10];

	if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_LOCATION, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registryKey, &result) != ERROR_SUCCESS) {

		_tprintf(TEXT("Erro ao criar/abrir chave (%d)\n"), GetLastError());
		return;
	}
	else
	{
		if (result == REG_OPENED_EXISTING_KEY) {
			_tprintf(TEXT("Chave: HKEY_CURRENT_USER\\%s aberta\n"), REGISTRY_LOCATION);

			for (int i = 0; i < MAX_SCORES; i++) {
				
				_stprintf_s(keyName, 10, TEXT("%d"), i);

				if (RegQueryValueEx(registryKey, keyName, NULL, &dwType, (LPBYTE)&highscore, &sizeOfStruct) != ERROR_SUCCESS) {
					break;
				}

				_tprintf(TEXT("ID obtido: %d\nNome Obtido: %s\nScore: %d\n\n\n"), i, highscore.playerName, highscore.score);
				//TODO: devolve isto! 

			}
		}
	}
	RegCloseKey(registryKey);
}