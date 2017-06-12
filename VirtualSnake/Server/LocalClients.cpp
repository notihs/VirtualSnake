#include "LocalClients.h" 



//TCHAR keyPressed;

DWORD WINAPI waitForLocalClients(LPVOID param) {

	Game* game = (Game *)param;

	int bufferKeyPosition = -1; 
	
	while (1) {
		WaitForSingleObject(hEventNewClient, INFINITE);
		
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (!game->snake[i]->isActive) {
				bufferKeyPosition = i;
				//ptrKeysInMemory[i] = 'o';
				game->activePlayers++;
				game->snake[i]->isActive = true;
				break;
			}
		}
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readKeyPressedByLocalClient, (LPVOID)game->snake[bufferKeyPosition], 0, NULL);
		if (bufferKeyPosition != -1) {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveSnakeToDirectionLocal, (LPVOID)game->snake[bufferKeyPosition], 0, NULL);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)readKeyPressedByLocalClient, (LPVOID)game->snake[bufferKeyPosition], 0, NULL);
			ResetEvent(hEventNewClient);
		}
		else {
			tcout << TEXT("\n\nNao aceita mais clientes");
		}
		//bufferKeyPosition++;
		//game->activePlayers = bufferKeyPosition; 
	}
}

DWORD WINAPI moveSnakeToDirectionLocal(LPVOID param) {
	
	Snake * snake = (Snake *)param;

	WaitForSingleObject(hEventGameStarted, INFINITE);

	while (snake->alive) { 

		tryToMoveSnake(snake->id, snake->direction);
		Sleep(BASE_SLEEP /snake->speed);
	}

	SetEvent(hEventSnakeDied[snake->id]); 

	return NULL;
}

DWORD WINAPI readKeyPressedByLocalClient(LPVOID param) {
	
	Snake * snake = (Snake *)param;

	while (1) {
		WaitForSingleObject(hEventKeyPressed[snake->id], INFINITE);
		WaitForSingleObject(hMutexWritingKey[snake->id], INFINITE);

		TCHAR key = ptrKeysInMemory[snake->id];
		tcout << TEXT("[LOCAL CLIENT] tecla recebida: ") << key << endl;

		
		if (snake->isDrunk) {
			key = swapKeys(key);
		}

		if (validMovement(snake->id, key)) {
			snake->direction = key; 
		}

		//tryToMoveSnake(snake->id, tecla);

		ReleaseMutex(hMutexWritingKey[snake->id]);
		ResetEvent(hEventKeyPressed[snake->id]);
	}
	

	return NULL;
}

