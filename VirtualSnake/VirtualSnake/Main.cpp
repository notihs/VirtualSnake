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

void remoteGame(TCHAR **map) {

	TCHAR ip[IPSIZE];
	TCHAR buffer[256];
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
			//exit(-1);
		}

		if (!WaitNamedPipe(pipeReadName, NMPWAIT_WAIT_FOREVER)) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeReadName);
			//exit(-1);
		}

		_tprintf(TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n"));
		hPipeWrite = CreateFile(pipeWriteName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipeWrite == NULL) {
			_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), pipeWriteName);
			//exit(-1);
		}

		TCHAR *teclas = chooseKeys(); 
		TCHAR * key = (TCHAR *)malloc(sizeof(TCHAR));

		WaitForSingleObject(hEventGameStarted, INFINITE);

		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readFromPipe, (LPVOID)map, 0, NULL);

		do {
		
			//_tprintf(TEXT("\nMensagem: \n->"));
			//_fgetts(buffer, 256, stdin);

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
	TCHAR key;
	BOOL ret;
	TCHAR buffer[256];

	/*hPipeRead = CreateFile(pipeReadName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipeRead == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), pipeReadName);
		//exit(-1);
	}

	while (1) {
		ret = ReadFile(hPipeRead, &key, sizeof(TCHAR), &n, NULL);
		buffer[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		tcout << TEXT("tecla recebida: ") << key;
	}
	*/



	/*SECURITY_ATTRIBUTES sa;

	Security(sa);

	hPipeRead = CreateNamedPipe(PIPE_CLIENT, PIPE_ACCESS_INBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
		| PIPE_READMODE_MESSAGE, N_PIPES, sizeof(TCHAR), sizeof(TCHAR), 1000, &sa);

	if (hPipeRead == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("Erro na ligação ao cliente!"));
		exit(-1);
	}

	_tprintf(TEXT("[SERVIDOR] Esperar ligação de um cliente... (ConnectNamedPipe)\n"));
	ConnectNamedPipe(hPipeRead, NULL);*/

	return NULL;
}
/*
void Security(SECURITY_ATTRIBUTES &sa)
{
	//SECURITY_ATTRIBUTES sa;
	PSECURITY_DESCRIPTOR pSD;
	PACL pAcl;
	EXPLICIT_ACCESS ea;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	TCHAR str[256];
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (pSD == NULL) {
		_tprintf(TEXT("Erro LocalAlloc!!!(%d)\n"), GetLastError());
		return;
	}
	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {
		_tprintf(TEXT("Erro IniSec!!!(%d)\n"), GetLastError());;
		return;
	}
	// Create a well-known SID for the Everyone group.
	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) {
		_tprintf(TEXT("AllocateAndInitializeSid() error %u"), GetLastError());
		Cleanup(pEveryoneSID, pAdminSID, NULL, pSD);
	}
	else {
		_tprintf(TEXT("AllocateAndInitializeSid() for the Everyone group is OK"));
		ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
		ea.grfAccessPermissions = GENERIC_ALL;
		ea.grfAccessMode = SET_ACCESS;
		ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
		ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea.Trustee.ptstrName = (LPTSTR)pEveryoneSID;
		if (SetEntriesInAcl(1, &ea, NULL, &pAcl) != ERROR_SUCCESS) {
			_tprintf(TEXT("Erro SetAcl!!!(%d)\n"), GetLastError());
			return;
		}
		if (!SetSecurityDescriptorDacl(pSD, TRUE, pAcl, FALSE)) {
			_tprintf(TEXT("Erro IniSec!!!(%d)\n"), GetLastError());
			return;
		}
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = TRUE;
		if (!CreateDirectory(TEXT("c:\\teste3"), &sa))
			_tprintf(TEXT("Erro CreateDir!!!(%d)\n"), GetLastError());
		else
			_tprintf(TEXT("Directory c:\\teste3 successfuly created\n"));
	}
}

void Cleanup(PSID pEveryoneSID, PSID pAdminSID, PACL pACL, PSECURITY_DESCRIPTOR pSD)
{
	if (pEveryoneSID)
		FreeSid(pEveryoneSID);
	if (pAdminSID)
		FreeSid(pAdminSID);
	if (pACL)
		LocalFree(pACL);
	if (pSD)
		LocalFree(pSD);
}
*/
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

