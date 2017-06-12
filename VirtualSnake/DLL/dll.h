#pragma once
//DLL.h
// O bloco ifdef seguinte é o modo standard de criar macros que tornam a exportação de
// funções e variáveis mais simples. Todos os ficheiros neste projeto DLL são
// compilados com o símbolo DLL_IMP_EXPORTS definido. Este símbolo não deve ser definido
// em nenhum projeto que use a DLL. Desta forma, qualquer outro projeto que inclua este
// este ficheiro irá ver as funções e variáveis DLL_IMP_API como sendo importadas de uma
// DLL.
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "CommonConstants.h"

using namespace std;

//Definir uma constante para facilitar a leitura do protótipo da função
//Este .h deve ser incluído no projeto que o vai usar (modo implícito)
//#define TAM 256
//Esta macro é definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS não definida) onde DLL_IMP é o nome deste projeto
/*
#define TAM 100
#define MAP_ROWS 10
#define MAP_COLUMNS 20
#define BUFFER_MAPPING (MAP_ROWS * MAP_COLUMNS)
#define MAX_PLAYERS 4*/

#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#define tstring wstring

#else
#define tcout cout
#define tcin cin
#define tstring string
#endif


	 
	extern DLL_IMP_API HANDLE hEventNewClient;
	extern DLL_IMP_API HANDLE hEventKeyPressed[MAX_PLAYERS];
	extern DLL_IMP_API HANDLE hMutexWritingKey[MAX_PLAYERS];
	extern DLL_IMP_API TCHAR *ptrKeysInMemory; 
	extern DLL_IMP_API HANDLE hEventGameStarted;
	extern DLL_IMP_API HANDLE hEventServerIsOnline;
	extern DLL_IMP_API HANDLE hEventSnakeDied[MAX_PLAYERS];

	DLL_IMP_API void initArrayOfKeys();
	DLL_IMP_API void writeMapInMemory(TCHAR ** map);

	DLL_IMP_API bool readMapInMemory(TCHAR **);
	DLL_IMP_API int getOwnKeyArrayPosition();
	DLL_IMP_API void newKeyPressed(TCHAR tecla);

	//UTILS
	DLL_IMP_API void initSynchHandles();
	DLL_IMP_API void destroyMap();
	DLL_IMP_API void readKeys();
	DLL_IMP_API void initMemory();

