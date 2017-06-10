#include "RemoteClient.h"

HANDLE hPipeRead[MAX_PLAYERS];
HANDLE hPipeWrite[MAX_PLAYERS];
TCHAR buffer[256];

typedef struct SnakeAndPipe{
	Snake *snake;
	//HANDLE pipe;
	int idPipe;
}SnakeAndPipe;

void initPipes() {

	for (int i = 0; i < MAX_PLAYERS; i++) {

		hPipeRead[i] = INVALID_HANDLE_VALUE;
		hPipeWrite[i] = INVALID_HANDLE_VALUE;
	}

}

DWORD WINAPI waitForRemoteClients(LPVOID param) {

	Game* game = (Game *)param;
	SnakeAndPipe * auxStruct = (SnakeAndPipe *)malloc(sizeof(SnakeAndPipe));
	SECURITY_ATTRIBUTES sa;

	Security(sa);


	int bufferKeyPosition = -1;
	

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (hPipeRead[i] != INVALID_HANDLE_VALUE && hPipeWrite[i] != INVALID_HANDLE_VALUE) {
			hPipeRead[i] = CreateNamedPipe(PIPE_SERVER, PIPE_ACCESS_INBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
				| PIPE_READMODE_MESSAGE, N_PIPES, sizeof(TCHAR), sizeof(TCHAR), 1000, &sa);

			hPipeWrite[i] = CreateNamedPipe(PIPE_CLIENT, PIPE_ACCESS_OUTBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
				| PIPE_READMODE_MESSAGE, N_PIPES, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), 1000, &sa);

			if (hPipeRead[i] == INVALID_HANDLE_VALUE || hPipeWrite[i] == INVALID_HANDLE_VALUE) {
				_tprintf(TEXT("Erro na ligação ao cliente!"));
				exit(-1);
			}

			_tprintf(TEXT("[SERVIDOR] A espera de clientes! (ConnectNamedPipe)\n\n"));
			ConnectNamedPipe(hPipeRead[i], NULL); 
			ConnectNamedPipe(hPipeWrite[i], NULL);

			for (int j = 0; j < MAX_PLAYERS; j++) {
				if (!game->snake[j]->isActive) {
					bufferKeyPosition = j;
					ptrKeysInMemory[j] = 'o';
					game->activePlayers++;
					game->snake[j]->isActive = true;
					//auxStruct->pipe = hPipeRead[i];
					auxStruct->idPipe = i;
					auxStruct->snake = game->snake[bufferKeyPosition];
					//hPipeRead[bufferKeyPosition] = hPipeRead[i];
					break;
				}
			}

			tcout << TEXT("[NAMED PIPE] Um cliente ligou-se!") << endl;

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readKeyFromRemoteClient, (LPVOID)auxStruct, 0, NULL); //TODO: add another thread to write
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveSnakeToDirectionRemote, (LPVOID)auxStruct, 0, NULL);

			//_tprintf(TEXT("Done"));
		}
		else if (i == MAX_PLAYERS) {
			_tprintf(TEXT("[SERVER] Numero maximo de clientes ligados!"));
		}
	}
	return NULL;
}




DWORD WINAPI readKeyFromRemoteClient(LPVOID param) {

	SnakeAndPipe * auxStruct = (SnakeAndPipe *)param;
	HANDLE pipe = hPipeRead[auxStruct->idPipe];
	Snake * snake = auxStruct->snake;


	BOOL ret;
	DWORD n;
	TCHAR *key;

	key = (TCHAR *)malloc(sizeof(TCHAR));

	while (1) {
		//tcout << TEXT("A espera de teclas!");
		ret = ReadFile(pipe, key, sizeof(TCHAR), &n, NULL);
		buffer[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		tcout << TEXT("[NAMED PIPE]Tecla recebida: ") << key[0] << endl;

		if (validMovement(snake->id, key[0])) {
			snake->direction = key[0]; //TODO: add mutex here or something
		}
	}

	return NULL;
}

DWORD WINAPI moveSnakeToDirectionRemote(LPVOID param) {

	SnakeAndPipe * auxStruct = (SnakeAndPipe *)param;
	HANDLE pipe = hPipeWrite[auxStruct->idPipe];
	Snake * snake = auxStruct->snake;
	DWORD n;

	WaitForSingleObject(hEventGameStarted, INFINITE);

	//TODO: fazer connect aqui?

	TCHAR ** map;
	//TCHAR *key;

	//key = (TCHAR *)malloc(sizeof(TCHAR));

	//key[0] = 'x';

	map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
	if (map == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
		exit(0);
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
		if (map[i] == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
			exit(0);
		}
	}


	while (1) {

		tryToMoveSnake(snake->id, snake->direction); 
		readMapInMemory(map);

		tcout << TEXT("O que o map tem la dentro! ") << endl;
		showMap(map);

		TCHAR aux[MAP_ROWS][MAP_COLUMNS]; //TODO: isto esta estrnho xD se nao der doutra maneira, fica assim

		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				aux[i][j] = map[i][j];
			}
		}

		if (!WriteFile(pipe, aux, sizeof(TCHAR[MAP_ROWS][MAP_COLUMNS]), &n, NULL)) {
			_tprintf(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
		}

		tcout << TEXT("Mandei ") << n << TEXT(" bytes");
		
		Sleep(2000 / snake->speed);
	}

	return NULL;
}


//SEGURANCA

void Security(SECURITY_ATTRIBUTES &sa)
{
	//SECURITY_ATTRIBUTES sa;
	PSECURITY_DESCRIPTOR pSD;
	PACL pAcl;
	EXPLICIT_ACCESS ea;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	//TCHAR str[256];
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
			_tprintf(TEXT("\nErro CreateDir!!!(%d)\n"), GetLastError());
		else
			_tprintf(TEXT("\nDirectory c:\\teste3 successfuly created\n"));
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