#include "Map.h"
#include "Clients.h"

using namespace std;


int _tmain(int argc, TCHAR *argv[]) {


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT); 
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
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

	TCHAR tecla[256];

	//TODO: guardar os arrays de TCHAR 
	createMap(); //Map.cpp

	_tprintf(TEXT("[SERVER] Servidor lancado! Pressione uma tecla para terminar"));
	_fgetts(tecla, 256, stdin);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForClients, NULL, 0, NULL); //Clients.cpp
}