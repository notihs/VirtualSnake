#include "Main.h"


using namespace std;

#define BUFFER 256
#define IPSIZE 20

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

	TCHAR tecla[BUFFER];
	TCHAR user[BUFFER];
	TCHAR ** map; 
	bool stayHere;

	//_tprintf(TEXT("Bem-vindo ao Virtual Snake! Escolha o seu username! \n->"));
	//_fgetts(user, BUFFER, stdin); //NOTA: username vai com \n no fim

	initSynchHandles();

	/*if (WaitForSingleObject(hEventServerIsOnline, WAIT_TIMEOUT) != WAIT_OBJECT_0) { //TODO: fix this, or ignore
		tcout << TEXT("O servidor nao se encontra online! Tente mais tarde!");
		tcout << TEXT("\n\nPressione uma tecla para continuar");
		_fgetts(user, BUFFER, stdin);
		return 0;
	}*/

	if (!WaitForSingleObject(hEventGameStarted, WAIT_TIMEOUT)) {
		tcout << TEXT("Já existe um jogo a decorrer! A aplicação vai fechar");
		tcout << TEXT("\n\nPressione uma tecla para continuar");
		_fgetts(user, BUFFER, stdin);
		return 0;
	}

	map = initMalloc();

	do {
		stayHere = false;

		_tprintf(TEXT("\nEscolha o tipo de jogo!"));
		_tprintf(TEXT("\n1 - Jogo Local"));
		_tprintf(TEXT("\n2 - Jogo Remoto"));
		_tprintf(TEXT("\n3 - Sair  \n->"));
		_fgetts(tecla, BUFFER, stdin);

		_tprintf(TEXT("\n"));

		tecla[_tcscspn(tecla, TEXT("\n"))] = '\0'; // elimina o \n da string

		if (!_tcscmp(tecla, TEXT("1"))) {
			_tprintf(TEXT("Aguarde pelo Jogo Local!\n\n"));
			localGame(map);			
		}
		else if (!_tcscmp(tecla, TEXT("2"))) {
			_tprintf(TEXT("Aguarde pelo Jogo Remoto!")); 
			remoteGame();
		}
		else if (!_tcscmp(tecla, TEXT("3"))) {
			_tprintf(TEXT("Obrigado por jogar!"));
			return 0;
		}
		else {
			_tprintf(TEXT("Escolha uma opção válida!\n"));
			stayHere = true;
		}

	} while (1); //TODO: while(stayHere)



	return 0;
}

void localGame(TCHAR ** map) {

	TCHAR up, down, left, right;
	TCHAR res;

	initMemory();
	
	//tcout << TEXT("Posicao livre: ") << getOwnKeyArrayPosition();

	if(getOwnKeyArrayPosition() == -1){
		tcout << TEXT("Já nao ha posicoes livres para jogar localmente! Pressione uma tecla para terminar");
		_getch();
		return;
	}

	TCHAR *teclas = chooseKeys();
	/*
	do {
		tcout << TEXT("Escolha as teclas para se movimentar: ");
		tcout << TEXT("\n\nTecla para cima: ");
		up = _getch();
		do {
			tcout << TEXT("\nTecla para baixo: ");
			down = _getch();
		} while (down == up);
		do {
			tcout << TEXT("\nTecla para esquerda: ");
			left = _getch();
		} while (left == down || left == up);
		do {
			tcout << TEXT("\nTecla para direita: ");
			right = _getch();
		} while (right == down || right == up || right == left);

		tcout << TEXT("\n\nTeclas escolhidas: ");
		tcout << endl << TEXT("Cima: ") << up;
		tcout << endl << TEXT("Baixo: ") << down;
		tcout << endl << TEXT("Esquerda: ") << left;
		tcout << endl << TEXT("Direita: ") << right;
		tcout << endl << endl << TEXT("Confirma estas teclas? (S/N)");
		res = _getch();

	} while (_totupper(res) != 'S');*/

	tcout << endl << endl << TEXT("Aguarde pelo inicio do jogo!");

	WaitForSingleObject(hEventGameStarted, INFINITE);
	
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForMapChanges, (LPVOID)map, 0, NULL);

	//readMapInMemory(); //DLL

	while (1) {

		/*tcout << endl << TEXT("Movimento:");
						   //TCHAR key = _gettchar();
		TCHAR key = _getch(); 
		TCHAR aux;

		if (key == up) {
			aux = 'u';
		}
		else if (key == down) {
			aux = 'd';
		}
		else if (key == left) {
			aux = 'l';
		}
		else if (key == right) {
			aux = 'r';
		}
		else {
			continue;
		}*/

		TCHAR aux = keyPress(teclas);

		if (aux != ' ') {
			newKeyPressed(aux);
		}

		

		//readMapInMemory(); //TODO: este read tem que estar numa thread!
	}
}

void remoteGame() {

	TCHAR ip[IPSIZE];
	TCHAR pipeServer[BUFFER];
	HANDLE hPipe;
	TCHAR buffer[256];
	DWORD n;

	while (1) {
		
		_tprintf(TEXT("\n\n(Insira \"sair\" a qualquer momento para voltar ao ecrã inicial!)"));
		_tprintf(TEXT("\nInsira o ip do servidor remoto : \n->"));
		_fgetts(ip, IPSIZE, stdin);

		ip[_tcscspn(ip, TEXT("\n"))] = '\0';

		_stprintf_s(pipeServer, BUFFER, TEXT("\\\\%s\\pipe\\server"), ip);

		//tcout << pipeServer;

		_tprintf(TEXT("[CLIENTE]Esperar pelo pipe '%s'(WaitNamedPipe)\n"), pipeServer);
		if (!WaitNamedPipe(pipeServer, NMPWAIT_WAIT_FOREVER)) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeServer);
			//exit(-1);
		}

		_tprintf(TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n"));
		hPipe = CreateFile(pipeServer, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipe == NULL) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), pipeServer);
			//exit(-1);
		}

		do {
		
			_tprintf(TEXT("\nMensagem: \n->"));
			_fgetts(buffer, 256, stdin);

			if (!WriteFile(hPipe, buffer, _tcslen(buffer) * sizeof(TCHAR), &n, NULL)) {
				_tprintf(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
			}


			//tcout << TEXT("\n\nbuffer ->") << buffer;

		} while (_tcscmp(buffer, TEXT("fim")));
		//TODO: fazer algo apos a conexao ser efetuada com sucesso
	
	}
}

DWORD WINAPI WaitForMapChanges(LPVOID param) {

	TCHAR ** map = (TCHAR **)param;

	while (1) {
		readMapInMemory(map);
	}

	return NULL;
	

}

TCHAR ** initMalloc() {

	TCHAR ** map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
	if (map == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
		exit(0);
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
		if (map == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
			exit(0);
		}
	}

	return map;
}

