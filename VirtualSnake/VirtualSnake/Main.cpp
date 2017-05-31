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
	bool stayHere;

	_tprintf(TEXT("Bem-vindo ao Virtual Snake! Escolha o seu username! \n->"));
	_fgetts(user, BUFFER, stdin); //NOTA: username vai com \n no fim

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
			localGame();			
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

void localGame() {


	TCHAR up, down, left, right;
	TCHAR res;

	initMemory();
	initSynchHandles();
	//tcout << TEXT("Posicao livre: ") << getOwnKeyArrayPosition();

	if(getOwnKeyArrayPosition() == -1){
		tcout << TEXT("Já nao ha posicoes livres para jogar localmente! Pressione uma tecla para terminar");
		_getch();
		return;
	}

	//TODO: se houver tempo, por de maneira a que as arrow keys etc, funcionem. E necessairo usar as virtual key codes do windows
	do {
		tcout << TEXT("Escolha as teclas para se movimentar: ");
		tcout << TEXT("\n\nTecla para cima: ");
		up = _getch();
		tcout << TEXT("\nTecla para baixo: ");
		down = _getch();
		tcout << TEXT("\nTecla para esquerda: ");
		left = _getch();
		tcout << TEXT("\nTecla para direita: ");
		right = _getch();

		tcout << TEXT("\n\nTeclas escolhidas: ");
		tcout << endl << TEXT("Cima: ") << up;
		tcout << endl << TEXT("Baixo: ") << down;
		tcout << endl << TEXT("Esquerda: ") << left;
		tcout << endl << TEXT("Direita: ") << right;
		tcout << endl << endl << TEXT("Confirma estas teclas? (S/N)");
		res = _getch();

	} while (_totupper(res)!='S');

	//TODO: nao pode logo vir para aqui! aguardar que o server de permissao para comecar o jogo!
	tcout << endl << endl << TEXT("Aguarde pelo inicio do jogo!");

	WaitForSingleObject(hEventGameStarted, INFINITE);
	

	while (1) {
		readMapInMemory(); //DLL

		tcout << endl << TEXT("Movimento:");
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
		}

		newKeyPressed(aux);
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

		_tprintf(TEXT("[CLIENTE] Ligação ao escritor... (CreateFile)\n"));
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