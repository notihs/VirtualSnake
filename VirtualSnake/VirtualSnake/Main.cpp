#include "Main.h"


using namespace std;

#define BUFFER 256
#define IPSIZE 20

TCHAR pipeWriteName[BUFFER];
TCHAR pipeReadName[BUFFER];
HANDLE hPipeWrite;
HANDLE hPipeRead;

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
			remoteGame(map);
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

//	TCHAR up, down, left, right;
	//TCHAR res;

	initMemory();
	
	//tcout << TEXT("Posicao livre: ") << getOwnKeyArrayPosition();

	if(getOwnKeyArrayPosition() == -1){
		tcout << TEXT("Já nao ha posicoes livres para jogar localmente! Pressione uma tecla para terminar");
		_getch();
		return;
	}

	TCHAR *teclas = chooseKeys();
	

	tcout << endl << endl << TEXT("Aguarde pelo inicio do jogo!");

	WaitForSingleObject(hEventGameStarted, INFINITE);
	
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForMapChanges, (LPVOID)map, 0, NULL);

	while (1) {

		TCHAR aux = keyPress(teclas);

		if (aux != ' ') {
			newKeyPressed(aux);
		}

	}
}

void remoteGame(TCHAR **map) {

	TCHAR ip[IPSIZE];
	//TCHAR buffer[256];
	DWORD n;

	while (1) {
		
		_tprintf(TEXT("\n\n(Insira \"sair\" a qualquer momento para voltar ao ecrã inicial!)"));
		_tprintf(TEXT("\nInsira o ip do servidor remoto : \n->"));
		_fgetts(ip, IPSIZE, stdin);

		ip[_tcscspn(ip, TEXT("\n"))] = '\0';

		_stprintf_s(pipeWriteName, BUFFER, TEXT("\\\\%s\\pipe\\server"), ip);
		_stprintf_s(pipeReadName, BUFFER, TEXT("\\\\%s\\pipe\\client"), ip);
		//TODO: add pipeReadName here too!


		//tcout << pipeWriteName;

		_tprintf(TEXT("[CLIENTE]Esperar pelo pipe '%s'(WaitNamedPipe)\n"), pipeWriteName);
		_tprintf(TEXT("[CLIENTE]Esperar pelo pipe '%s'(WaitNamedPipe)\n"), pipeReadName);
		if (!WaitNamedPipe(pipeWriteName, NMPWAIT_WAIT_FOREVER)) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeWriteName);
			exit(-1);
		}

		if (!WaitNamedPipe(pipeReadName, NMPWAIT_WAIT_FOREVER)) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeReadName);
			exit(-1);
		}

		_tprintf(TEXT("[CLIENTE] A efetuar ligação ao servidor... (CreateFile)\n"));
		hPipeWrite = CreateFile(pipeWriteName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipeWrite == NULL) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), pipeWriteName);
			exit(-1);
		}

		hPipeRead = CreateFile(pipeReadName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipeRead == NULL) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), pipeReadName);
			exit(-1);
		}

		TCHAR *teclas = chooseKeys(); 
		TCHAR * key = (TCHAR *)malloc(sizeof(TCHAR));

		tcout << endl << endl << TEXT("Aguarde pelo inicio do jogo!");

		WaitForSingleObject(hEventGameStarted, INFINITE);

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readFromPipe, (LPVOID)map, 0, NULL);

		do {
		

			TCHAR aux = keyPress(teclas);

			if (aux != ' ') {

				key[0] = aux;

				if (!WriteFile(hPipeWrite, key, sizeof(TCHAR), &n, NULL)) {
					_tprintf(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				}

			}
			//tcout << TEXT("\n\nbuffer ->") << buffer;

		} while (1);
		//TODO: fazer algo apos a conexao ser efetuada com sucesso
	
	}
}

DWORD WINAPI readFromPipe(LPVOID param) {

	TCHAR ** map = (TCHAR **)param;
	DWORD n;
//	TCHAR key[1];
	BOOL ret;
//	TCHAR buffer[256];
	TCHAR aux[MAP_ROWS][MAP_COLUMNS];

	//TODO: passa-se algo aqui errado!
	while (1) {
		ret = ReadFile(hPipeRead, aux, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), &n, NULL);
		//map[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		//tcout << TEXT("tecla recebida: ") << key;

		system("cls");

		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				tcout << aux[i][j];
			}
			tcout << endl;
		}
	}
	

	return NULL;
}


DWORD WINAPI WaitForMapChanges(LPVOID param) {

	TCHAR ** map = (TCHAR **)param;

	while (1) {

		readMapInMemory(map);

		system("cls");		//clrscr();

		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				tcout << map[i][j];
			}
			tcout << endl;
		}
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

