#include "Server.h"

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
	//TCHAR **map;
	Game game;
	 
	for (int i = 0; i < MAX_PLAYERS; i++) {
		game.snake[i] = initOneSnake(i); //Snake.h
	}
	game.map = createMap(); //Map.cpp

	//Local clients Configuration

	initMemory();
	initSynchHandles();
	initArrayOfKeys();
	writeMapInMemory(game.map); //DLL

	
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, (LPVOID)hEventNewClient, 0, NULL); //Clients.cpp
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, NULL, 0, NULL);



	//Remote Clients configuration!

	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForRemoteClients, NULL, 0, NULL); //Clients.cpp



	while (1) {
		_tprintf(TEXT("[SERVER] Servidor lancado! Pressione uma tecla para terminar"));
		_fgetts(tecla, 256, stdin);

		readKeys();
	}
	destroyMap();
}


