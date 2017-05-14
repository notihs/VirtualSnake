#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "Main.h"
#include "..\DLL\dll.h"

using namespace std;

int _tmain(int argc, TCHAR *argv[]) {
	
	#ifdef UNICODE
	#define tcout wcout
	#define tcin wcin
	#define tstring wstring

	#else
	#define tcout cout
	#define tcin cin
	#define tstring string
	#endif

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	//mapInMemory();

	readMapInMemory();

	return 0;
}


//void mapInMemory() {
//
////	HANDLE hFile;
//	HANDLE canWrite;
//	HANDLE canRead;
//	HANDLE hMemory;
//	TCHAR *writeSemaphoreName = TEXT("Write Semaphore");
//	TCHAR *readSemaphoreName = TEXT("Read Semaphore");
//	TCHAR *memoryName = TEXT("Memory");
//	TCHAR(*PtrMemoria)[MAX_CLIENTS][BUFFER_MAPPING];
//
//	//hFile = CreateFile(TEXT("teste.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	canWrite = CreateSemaphore(NULL, MAX_CLIENTS, MAX_CLIENTS, writeSemaphoreName);
//	canRead = CreateSemaphore(NULL, 0, MAX_CLIENTS, readSemaphoreName);
//	hMemory = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0
//		, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]), memoryName);
//
//	if (canWrite == NULL || canRead == NULL || hMemory == NULL) {
//		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
//		return;
//	}
//
//	PtrMemoria = (TCHAR(*)[MAX_CLIENTS][BUFFER_MAPPING])MapViewOfFile(hMemory
//		, FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[MAX_CLIENTS][BUFFER_MAPPING]));
//	if (PtrMemoria == NULL) {
//		_tprintf(TEXT("[Erro]Mapeamento da memória partilhada(%d)\n"), GetLastError());
//		return;
//	}
//
//	for (int i = 0; i < 100; i++)
//	{
//		WaitForSingleObject(canRead, INFINITE);
//		_tprintf((*PtrMemoria)[i % MAX_CLIENTS]); // Reader reads data
//		ReleaseSemaphore(canWrite, 1, NULL);
//	}
//	UnmapViewOfFile(PtrMemoria);
//	CloseHandle(canWrite);
//	CloseHandle(canRead);
//	CloseHandle(hMemory);
//
//	return;
//}
