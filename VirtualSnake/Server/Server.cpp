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
	Game *copyOfGame = (Game *)malloc(sizeof(Game));

	//Game initialization

	initGame();
	copyOfGame = getCurrentGame();
	//Local clients Configuration

	initMemory();
	initSynchHandles();
	initArrayOfKeys();
	
	
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, (LPVOID)hEventNewClient, 0, NULL); //Clients.cpp
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, (LPVOID)copyOfGame, 0, NULL); //TODO: isto tem a copia do jogo! atencao!



	//Remote Clients configuration!

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForRemoteClients, NULL, 0, NULL); //Clients.cpp

	_tprintf(TEXT("[SERVER] Servidor lancado! Pressione uma tecla para comecar o jogo!"));
	_fgetts(tecla, 256, stdin);
	
	startGame();
	copyOfGame = getCurrentGame();


	tcout << endl << endl;
	showSnakeInfo();
	tcout << endl << endl;
	showSnakeInfo();
	tcout << endl << endl;

	writeMapInMemory(copyOfGame->map); //DLL
	SetEvent(hEventGameStarted); //TODO: Sera evento a anunciar a TODOS os utilizadores que ja comecou??

	while (1) {
		_tprintf(TEXT("[SERVER] Ciclo infinito! a ler keys nos buffers!"));
		_fgetts(tecla, 256, stdin);

		readKeys();
	}
	destroyMap();
}


