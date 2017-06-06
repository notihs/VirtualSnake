//DLL.cpp
#include <windows.h>
#include <stdio.h>
//#include "DLL.h"

#include "..\..\VirtualSnake\DLL\dll.h"
//Para verificar ao carregar a dll que a aplica��o ir� ocupar mais mem�ria
char ponteiro[40960];

//------------------------------------------------------


//EVENTOS
TCHAR eventKeyPressedName[TAM];
HANDLE hEventKeyPressed[MAX_PLAYERS];

TCHAR eventNewClientName[TAM] = TEXT("Event new client");
HANDLE hEventNewClient;

TCHAR eventGameStartedName[TAM] = TEXT("Event game started");
HANDLE hEventGameStarted;

TCHAR eventMapChangesName[TAM] = TEXT("Event map changes"); //TODO: deve ser um semaforo aqui! um evento nao funciona muito bem, acho..
HANDLE hEventMapChanges[MAX_PLAYERS];

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
TCHAR(*ptrKeysInMemory)[MAX_PLAYERS]; //LIST OF KEYS:
									  //e -> empty
									  //o -> occupied
									  //u -> up
									  //d -> down
									  //l -> left
									  //r -> right
void initSynchHandles() {

	for (int i = 0; i < MAX_PLAYERS; i++) {
		_stprintf_s(eventKeyPressedName, TAM, TEXT("Key Pressed Event %d"), i);
		_stprintf_s(mutexWritingKeyName, TAM, TEXT("Key Writing Mutex %d"), i);

		hEventKeyPressed[i] = CreateEvent(NULL, TRUE, FALSE, eventKeyPressedName);
		hEventMapChanges[i] = CreateEvent(NULL, TRUE, FALSE, eventMapChangesName);
		
		hMutexWritingKey[i] = CreateMutex(NULL, FALSE, mutexWritingKeyName);
		
		if (hEventKeyPressed[i] == NULL || hEventMapChanges[i] == NULL){
			_tprintf(TEXT("[Erro]Cria��o de eventos(%d)\n"), GetLastError());
			return;
		}
		else if (hMutexWritingKey[i] == NULL) {
			_tprintf(TEXT("[Erro]Cria��o de mutexes(%d)\n"), GetLastError());
			return;
		}

	}

	hEventGameStarted = CreateEvent(NULL, TRUE, FALSE, eventGameStartedName);

	hEventNewClient = CreateEvent(NULL, TRUE, FALSE, eventNewClientName); //Evento criado a FALSE 
																		  //Set -> mete a TRUE
																		  //Reset -> mete a FALSE

	if (hEventNewClient == NULL || hEventGameStarted == NULL) {
		_tprintf(TEXT("[Erro]Cria��o de evento(%d)\n"), GetLastError());
		return;
	}
}

void initMemory() {

	hKeysMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_PLAYERS]), keysInMemoryName);

	hMapMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), mapInMemoryName);

	if (hKeysMemory == NULL || hMapMemory == NULL) {
		_tprintf(TEXT("[Erro]Cria��o de objectos do Windows(%d)\n"), GetLastError());
		return;
	}

	ptrKeysInMemory = (TCHAR(*)[MAX_PLAYERS])MapViewOfFile(hKeysMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_PLAYERS]));

	ptrMapInMemory = (TCHAR(*)[MAP_ROWS][MAP_COLUMNS])MapViewOfFile(hMapMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]));

	if (ptrKeysInMemory == NULL || ptrMapInMemory == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da mem�ria partilhada(%d)\n"), GetLastError());
		return;
	}

}


void initArrayOfKeys() { //TODO: Em principio, nao deve haver problemas de sincronizacao

	for (int i = 0; i < MAX_PLAYERS; i++) {
		(*ptrKeysInMemory)[i] = 'e';
		tcout << TEXT("Posicao ") << i << TEXT(": ") << (*ptrKeysInMemory)[i] << endl;
	}

	return;
}

void readKeys() { //TODO: read only when event happens!

	for (int i = 0; i < MAX_PLAYERS; i++) {
		//(*ptrKeysInMemory)[i] = 'e';
		tcout << TEXT("Posicao ") << i << TEXT(": ") << (*ptrKeysInMemory)[i] << endl;
	}

}

void newKeyPressed(TCHAR tecla) { 
	WaitForSingleObject(hMutexWritingKey[ownId], INFINITE);

	(*ptrKeysInMemory)[ownId] = tecla;

	ReleaseMutex(hMutexWritingKey[ownId]); 
	SetEvent(hEventKeyPressed[ownId]); // Mete evento a TRUE

}

int getOwnKeyArrayPosition() {

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if ((*ptrKeysInMemory)[i] == 'e') {

			WaitForSingleObject(hMutexWritingKey[i], INFINITE);

			(*ptrKeysInMemory)[i] = 'o'; 
			
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
			tcout << (*ptrMapInMemory)[i][j];
		}
		tcout << endl;
	}

	for (int i = 0; i < MAX_PLAYERS;i++) {
		SetEvent(hEventMapChanges[i]);
	}
	
	return;
}

TCHAR** readMapInMemory() {

	TCHAR ** map; //TODO: isto so e reservado 1 vez no jogo todo! nao e em cada read!

	map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
	}

	WaitForSingleObject(hEventMapChanges[ownId], INFINITE);
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			map[i][j] = (*ptrMapInMemory)[i][j];
			tcout << map[i][j];
		}
		tcout << endl;
	}

	ResetEvent(hEventMapChanges[ownId]);
	return map;
}

//TODO: close all handles here!
void destroyMap() {
	UnmapViewOfFile(ptrMapInMemory);
	CloseHandle(hMapMemory);
}

