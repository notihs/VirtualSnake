//DLL.cpp
#include <windows.h>
#include <stdio.h>
//#include "DLL.h"

#include "..\..\VirtualSnake\DLL\dll.h"
//Para verificar ao carregar a dll que a aplicação irá ocupar mais memória
char ponteiro[40960];

//------------------------------------------------------

#define MAX_CLIENTS 4
#define MAP_ROWS 10
#define MAP_COLUMNS 20
//#define BUFFER_MAPPING 100
#define BUFFER_MAPPING (MAP_ROWS * MAP_COLUMNS)

HANDLE canWriteEvent[MAX_CLIENTS];
HANDLE canReadEvent[MAX_CLIENTS];
HANDLE canWrite;
HANDLE canRead;
HANDLE hMemory;
TCHAR writeEventName[100];// = TEXT("Write Event ");
TCHAR readEventName[100];
TCHAR *writeSemaphoreName = TEXT("Write Semaphore");
TCHAR *readSemaphoreName = TEXT("Read Semaphore");
TCHAR *memoryName = TEXT("Memory");
TCHAR(*PtrMemoria)[MAX_CLIENTS][BUFFER_MAPPING];


void generateEventNames() {

	for (int i = 0; i < MAX_CLIENTS; i++) {
		_stprintf_s(writeEventName, 100, TEXT("Write Event %d"), i);
		_stprintf_s(readEventName, 100, TEXT("Read Event %d"), i);
		//sprintf_s(name, 100,"Write Event %d", i);

		canWriteEvent[i] = CreateEvent(NULL, TRUE, TRUE, writeEventName);
		canReadEvent[i] = CreateEvent(NULL, TRUE, TRUE, readEventName);

		if (canWriteEvent[i] == NULL || canReadEvent[i] == NULL) {
			_tprintf(TEXT("[Erro]Criação de eventos(%d)\n"), GetLastError());
			return;
		}

		//_tprintf(writeEventName);
		//printf(name);
	}
}

//TODO: receber por arg o numero de clientes ligados e ON
void writeMapInMemory() {
	
	generateEventNames();

	canWrite = CreateSemaphore(NULL, MAX_CLIENTS, MAX_CLIENTS, writeSemaphoreName);
	canRead = CreateSemaphore(NULL, 0, MAX_CLIENTS, readSemaphoreName);
	hMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]), memoryName);

	if (canWrite == NULL || canRead == NULL || hMemory == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return;
	}

	PtrMemoria = (TCHAR(*)[MAX_CLIENTS][BUFFER_MAPPING])MapViewOfFile(hMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]));
	if (PtrMemoria == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
		return;
	}

	for (int i = 0; i < 100; i++)
	{
		//WaitForSingleObject(canWrite, INFINITE);
		WaitForMultipleObjects(MAX_CLIENTS, canWriteEvent, TRUE, INFINITE);
		_tprintf(TEXT("Escrever para buffer %i\n"), i);
		//for(int j = 0; j < MAX_CLIENTS; j++)
		_stprintf_s((*PtrMemoria)[i%MAX_CLIENTS], BUFFER_MAPPING, TEXT("Escritor-%i\n"), i); 
		//_stprintf_s((*PtrMemoria)[i%MAX_CLIENTS], BUFFER_MAPPING, TEXT("Escritor-%i\n"), i);
		ResetEvent(canWriteEvent[i%MAX_CLIENTS]);
		Sleep(1000);
		ReleaseSemaphore(canRead, 1, NULL);
	}
	UnmapViewOfFile(PtrMemoria);
	CloseHandle(canWrite);
	CloseHandle(canRead);
	CloseHandle(hMemory);

	return;
}

TCHAR* readMapInMemory() {

	generateEventNames();

	canWrite = CreateSemaphore(NULL, MAX_CLIENTS, MAX_CLIENTS, writeSemaphoreName);
	canRead = CreateSemaphore(NULL, 0, MAX_CLIENTS, readSemaphoreName);
	hMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
		, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]), memoryName);

	if (canWrite == NULL || canRead == NULL || hMemory == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return NULL;
	}

	PtrMemoria = (TCHAR(*)[MAX_CLIENTS][BUFFER_MAPPING])MapViewOfFile(hMemory
		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]));
	if (PtrMemoria == NULL) {
		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
		return NULL;
	}

	for (int i = 0;; ++i)
	{
		WaitForSingleObject(canRead, INFINITE);
		_tprintf((*PtrMemoria)[i % MAX_CLIENTS]); // Reader reads data
		//ReleaseSemaphore(canWrite, 1, NULL);
		SetEvent(canWriteEvent[i%MAX_CLIENTS]);
	}
	UnmapViewOfFile(PtrMemoria);
	CloseHandle(canWrite);
	CloseHandle(canRead);
	CloseHandle(hMemory);

	return TEXT("teste");
}

