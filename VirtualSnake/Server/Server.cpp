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

	tcout << TEXT("Primeira mensagem!");

	TCHAR tecla[256];
	Game *copyOfGame; //= (Game *)malloc(sizeof(Game));

	/*if (copyOfGame == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de CopyofGame* !");
		exit(0);
	}*/

	//Game initialization
	tcout << TEXT("initGame!");
	initGame();
	tcout << TEXT("FIM initGame!");

	tcout << TEXT("copyOfGame!");
	copyOfGame = getCurrentGame(); //TODO: neste momento esta a usar o getCurrentGame. Para usar uma copia, usar o metodo createCopyOfGame()
	//createCopyOfGame();
	tcout << TEXT("FIM copyOfGame!");


	//Local clients Configuration
	tcout << TEXT("initMemory!");
	initMemory();
	tcout << TEXT("FIM initMemory!");

	tcout << TEXT("initSynchHandles!");
	initSynchHandles();
	tcout << TEXT("FIM initSynchHandles!");
	tcout << TEXT("initArrayOfKeys!");
	initArrayOfKeys();
	tcout << TEXT("FIM initArrayOfKeys!");
	
	
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, (LPVOID)hEventNewClient, 0, NULL); //Clients.cpp
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForLocalClients, (LPVOID)copyOfGame, 0, NULL); //TODO: isto tem a copia do jogo! atencao!

	//Remote Clients configuration!

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForRemoteClients, NULL, 0, NULL); //Clients.cpp

	SetEvent(hEventServerIsOnline); //Avisa o cliente de que o server ja esta online e pronto a receber clientes

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


void createCopyOfGame(Game * copyOfGame) {

	//SO esta a criar o mapa, ainda

	copyOfGame->map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
	for (int i = 0; i < MAP_ROWS; i++) {

		copyOfGame->map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));

		for (int j = 0; j < MAP_COLUMNS; j++) {

			copyOfGame->map[i][j] = getCurrentGame()->map[i][j];

		}
	}

	tcout << TEXT("Copy of game!") << endl << endl;

	showMap(copyOfGame->map);

	tcout << TEXT("Current Game!") << endl << endl;

	showMap(getCurrentGame()->map);

	tcout << TEXT("Mudanca!") << endl << endl;

	copyOfGame->map[1][1] = '+';

	tcout << TEXT("Copy of game!") << endl << endl;

	showMap(copyOfGame->map);

	tcout << TEXT("Current Game!") << endl << endl;

	showMap(getCurrentGame()->map);

}

