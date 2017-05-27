//DLL.cpp
#include <windows.h>
#include <stdio.h>
//#include "DLL.h"

#include "..\..\VirtualSnake\DLL\dll.h"
//Para verificar ao carregar a dll que a aplicação irá ocupar mais memória
char ponteiro[40960];

//------------------------------------------------------


//EVENTOS
TCHAR eventKeyPressedName[TAM];
HANDLE hEventKeyPressed[MAX_PLAYERS];

TCHAR eventNewClientName[TAM] = TEXT("Event new client");
HANDLE hEventNewClient;

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
		//_stprintf_s(readEventName, 100, TEXT("Read Event %d"), i);
		//sprintf_s(name, 100,"Write Event %d", i);

		hEventKeyPressed[i] = CreateEvent(NULL, TRUE, FALSE, eventKeyPressedName);
		
		hMutexWritingKey[i] = CreateMutex(NULL, FALSE, mutexWritingKeyName);
		//canReadEvent[i] = CreateEvent(NULL, TRUE, TRUE, readEventName);

		//|| canReadEvent[i] == NULL) {
		if (hEventKeyPressed[i] == NULL){
			_tprintf(TEXT("[Erro]Criação de eventos(%d)\n"), GetLastError());
			return;
		}
		else if (hMutexWritingKey[i] == NULL) {
			_tprintf(TEXT("[Erro]Criação de mutexes(%d)\n"), GetLastError());
			return;
		}

		//_tprintf(eventKeyPressedName);
		//printf(name);
	}

	hEventNewClient = CreateEvent(NULL, TRUE, FALSE, eventNewClientName); //Evento criado a FALSE 
																		  //Set -> mete a TRUE
																		  //Reset -> mete a FALSE

	if (hEventNewClient == NULL) {
		_tprintf(TEXT("[Erro]Criação de evento(%d)\n"), GetLastError());
		return;
	}
}

void initMemory() {

	hKeysMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_PLAYERS]), keysInMemoryName);

	hMapMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), mapInMemoryName);

	if (hKeysMemory == NULL || hMapMemory == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return;
	}

	ptrKeysInMemory = (TCHAR(*)[MAX_PLAYERS])MapViewOfFile(hKeysMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_PLAYERS]));

	ptrMapInMemory = (TCHAR(*)[MAP_ROWS][MAP_COLUMNS])MapViewOfFile(hMapMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]));

	if (ptrKeysInMemory == NULL || ptrMapInMemory == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
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
	SetEvent(hEventKeyPressed);

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

	return;
}

TCHAR** readMapInMemory() {

	TCHAR ** map;

	map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			map[i][j] = (*ptrMapInMemory)[i][j];
			tcout << map[i][j];
		}
		tcout << endl;
	}

	return map;
}

//TODO: close all handles here!
void destroyMap() {
	UnmapViewOfFile(ptrMapInMemory);
	CloseHandle(hMapMemory);
}

