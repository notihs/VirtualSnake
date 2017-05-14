//DLL.cpp
#include <windows.h>
//#include "DLL.h"

#include "..\..\VirtualSnake\DLL\dll.h"
//Para verificar ao carregar a dll que a aplicação irá ocupar mais memória
char ponteiro[40960];
//Definição da variável global
int nDLL = 1234;
//Exportar a função para ser utilizada fora da DLL

int UmaString(void) {
	TCHAR str[TAM];
	_tprintf(TEXT("Dentro da Dll\nIntroduza uma frase:"));
	_fgetts(str, TAM, stdin);
	if (_tcslen(str) > 1) //Introduzir mais caracteres do que apenas <enter>
		return 1;
	else
		return 0;
}

void ImprimeInt(int x) {

	_tprintf(TEXT("Inteiro recebido: %d"), x);
}


//------------------------------------------------------

#define MAX_CLIENTS 4
#define BUFFER_MAPPING 100

void writeMapInMemory() {
	
	//HANDLE hFile;
	HANDLE canWrite;
	HANDLE canRead;
	HANDLE hMemory;
	TCHAR *writeSemaphoreName = TEXT("Write Semaphore");
	TCHAR *readSemaphoreName = TEXT("Read Semaphore");
	TCHAR *memoryName = TEXT("Memory");
	TCHAR(*PtrMemoria)[MAX_CLIENTS][BUFFER_MAPPING];

	//hFile = CreateFile(TEXT("teste.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
		WaitForSingleObject(canWrite, INFINITE);
		_tprintf(TEXT("Escrever para buffer %i\n"), i);
		//for(int j = 0; j < MAX_CLIENTS; j++)
		_stprintf_s((*PtrMemoria)[i%MAX_CLIENTS], BUFFER_MAPPING, TEXT("Escritor-%i\n"), i); //TODO: mudar o i%MAX_CLIENTS no array

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

	//	HANDLE hFile;
	HANDLE canWrite;
	HANDLE canRead;
	HANDLE hMemory;
	TCHAR *writeSemaphoreName = TEXT("Write Semaphore");
	TCHAR *readSemaphoreName = TEXT("Read Semaphore");
	TCHAR *memoryName = TEXT("Memory");
	TCHAR(*PtrMemoria)[MAX_CLIENTS][BUFFER_MAPPING];

	//hFile = CreateFile(TEXT("teste.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

	for (int i = 0; i < 100; i++)
	{
		WaitForSingleObject(canRead, INFINITE);
		_tprintf((*PtrMemoria)[i % MAX_CLIENTS]); // Reader reads data
		ReleaseSemaphore(canWrite, 1, NULL);
	}
	UnmapViewOfFile(PtrMemoria);
	CloseHandle(canWrite);
	CloseHandle(canRead);
	CloseHandle(hMemory);

	return TEXT("teste");
}

