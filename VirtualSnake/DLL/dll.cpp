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

	if (hKeysMemory == NULL || hMapMemory == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return;
	}

	ptrKeysInMemory = (TCHAR(*))MapViewOfFile(hKeysMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR) * MAX_PLAYERS);

	ptrMapInMemory = (TCHAR(*)[MAP_ROWS][MAP_COLUMNS])MapViewOfFile(hMapMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]));

	if (ptrKeysInMemory == NULL || ptrMapInMemory == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
		return;
	}

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
		tcout << TEXT("Posicao ") << i << TEXT(": ") << ptrKeysInMemory[i] << endl;
	}

}

void newKeyPressed(TCHAR tecla) { 
	WaitForSingleObject(hMutexWritingKey[ownId], INFINITE);

	ptrKeysInMemory[ownId] = tecla;

	ReleaseMutex(hMutexWritingKey[ownId]); 
	SetEvent(hEventKeyPressed[ownId]); // Mete evento a TRUE

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

bool readMapInMemory(TCHAR ** map) {

	//bool signaled = WaitForSingleObject(hEventSnakeDied[ownId], INFINITE);
	//tcout << TEXT("Evento da cobra morta ") << signaled;
	//if (WaitForSingleObject(hEventSnakeDied[ownId], 0) == WAIT_TIMEOUT) { //TODO: este evento nao esta bem!!!!
		WaitForSingleObject(hEventMapChanges[ownId], INFINITE);
		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				map[i][j] = (*ptrMapInMemory)[i][j];
				//tcout << map[i][j];
			}
			//tcout << endl;
		}

		ResetEvent(hEventMapChanges[ownId]);

		return true;
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

