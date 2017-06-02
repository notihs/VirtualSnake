#include "Clients.h" //TODO: este .h acede ao dll.h . Nao e muito correto! corrigir se possivel

//TODO: melhorar a maneira como o server vai receber as keys e tratar o que acontece + sincronizacao!
HANDLE pipeAux[MAX_PLAYERS];
TCHAR buffer[256];

DWORD WINAPI WaitForLocalClients(LPVOID param) {

	Game* game = (Game *)param;

	int bufferKeyPosition = 0; //TODO: melhorar a forma de obter qual o sitio dedicado a tecla de cada utilizador
	
	//TODO: deixar de aceitar clientes apos o jogo ter comecado!
	while (1) {
		WaitForSingleObject(hEventNewClient, INFINITE);
		
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readKeyPressedByLocalClient, (LPVOID) bufferKeyPosition, 0, NULL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readKeyPressedByLocalClient, (LPVOID)game->snake[bufferKeyPosition], 0, NULL);
		ResetEvent(hEventNewClient);
		bufferKeyPosition++;
		game->activePlayers = bufferKeyPosition;
	}
}

//TODO: mexer o mapa, guardar as alteracoes no game.map e sincronizar tudo (Falta um mutex?? para o readMap)
DWORD WINAPI readKeyPressedByLocalClient(LPVOID param) {
	
	//Game* game = (Game *)param;
	//int pos = (int)param;
	Snake * snake = (Snake *)param;

	while (1) {
		WaitForSingleObject(hEventKeyPressed[snake->id], INFINITE);
		WaitForSingleObject(hMutexWritingKey[snake->id], INFINITE);

		TCHAR tecla = (*ptrKeysInMemory)[snake->id];

		tcout << TEXT("\ntecla recebida: ") << tecla;

		//TODO: alterar o mapa aqui!
		tryToMoveSnake(snake->id, tecla);

		ReleaseMutex(hMutexWritingKey[snake->id]);
		ResetEvent(hEventKeyPressed[snake->id]);
	}
	

	return NULL;
}

//TODO: finish this
DWORD WINAPI WaitForRemoteClients(LPVOID param) {

	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (pipeAux[i] != INVALID_HANDLE_VALUE) {
			pipeAux[i] = CreateNamedPipe(PIPE_SERVER, PIPE_ACCESS_INBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
				| PIPE_READMODE_MESSAGE, N_PIPES, sizeof(buffer), sizeof(buffer), 1000, NULL);

			if (pipeAux[i] == INVALID_HANDLE_VALUE) {
				_tprintf(TEXT("Erro na ligação ao cliente!"));
				exit(-1);
			}

			_tprintf(TEXT("[SERVIDOR] Esperar ligação de um cliente... (ConnectNamedPipe)\n"));
			ConnectNamedPipe(pipeAux[i], NULL);

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadFromClientX, (LPVOID) pipeAux[i], 0, NULL);

			_tprintf(TEXT("Done"));
		}
		else if (i == MAX_PLAYERS) {
			_tprintf(TEXT("[SERVER] Numero maximo de clientes ligado!"));
		}
	}
	return NULL;
}

void initPipes() {

	for (int i = 0; i < MAX_PLAYERS; i++) {

		pipeAux[i] = INVALID_HANDLE_VALUE;
	}

}

DWORD WINAPI ReadFromClientX(LPVOID param) {

	HANDLE pipe = (HANDLE)param;
	
	BOOL ret;
	DWORD n;

	while (1) {
		ret = ReadFile(pipe, buffer, sizeof(buffer), &n, NULL);
		buffer[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		_tprintf(TEXT("[SERVER] Recebi %d bytes: '%s'... (ReadFile)\n"), n, buffer);
	}

	return NULL;
}