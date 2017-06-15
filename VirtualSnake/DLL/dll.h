#pragma once
//DLL.h
// O bloco ifdef seguinte � o modo standard de criar macros que tornam a exporta��o de
// fun��es e vari�veis mais simples. Todos os ficheiros neste projeto DLL s�o
// compilados com o s�mbolo DLL_IMP_EXPORTS definido. Este s�mbolo n�o deve ser definido
// em nenhum projeto que use a DLL. Desta forma, qualquer outro projeto que inclua este
// este ficheiro ir� ver as fun��es e vari�veis DLL_IMP_API como sendo importadas de uma
// DLL.
#include <windows.h>
#include <tchar.h>
//#include <iostream>
#include "CommonConstants.h"

//using namespace std;

//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projeto que o vai usar (modo impl�cito)
//#define TAM 256
//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projeto
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

extern DLL_IMP_API HANDLE hEventNewClient;
extern DLL_IMP_API HANDLE hEventKeyPressed[MAX_PLAYERS];
extern DLL_IMP_API HANDLE hMutexWritingKey[MAX_PLAYERS];
extern DLL_IMP_API TCHAR *ptrKeysInMemory;
extern DLL_IMP_API HANDLE hEventGameStarted;
extern DLL_IMP_API HANDLE hEventServerIsOnline;
extern DLL_IMP_API HANDLE hEventSnakeDied[MAX_PLAYERS];
extern DLL_IMP_API TCHAR(*ptrUsernameInMemory)[MAX_PLAYERS][TAM];

DLL_IMP_API void initArrayOfKeys();
DLL_IMP_API void writeMapInMemory(TCHAR ** map);
DLL_IMP_API void setOwnId(int);

DLL_IMP_API boolean readMapInMemory(TCHAR **);
DLL_IMP_API int getOwnKeyArrayPosition();
DLL_IMP_API void newKeyPressed(TCHAR tecla);

//UTILS
DLL_IMP_API void initSynchHandles();
DLL_IMP_API void destroyMap();
DLL_IMP_API void readKeys();
DLL_IMP_API void initMemory();


DLL_IMP_API void readAllKeysOnRegistry();

DLL_IMP_API void setUsername(TCHAR username[TAM]);
