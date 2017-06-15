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
				game->snake[i]->isActive = TRUE;
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
			//tcout << TEXT("\n\nNao aceita mais clientes");
			_tprintf(TEXT("\n\nNao aceita mais clientes"));
		}
		//bufferKeyPosition++;
		//game->activePlayers = bufferKeyPosition; 
	}
}

DWORD WINAPI moveSnakeToDirectionLocal(LPVOID param) {
	
	Snake * snake = (Snake *)param;

	WaitForSingleObject(hEventGameStarted, INFINITE);

	do { 

		tryToMoveSnake(snake->id, snake->direction);
		Sleep(BASE_SLEEP /snake->speed);

		if (snake->effectTimer > 0) {
			snake->effectTimer--;
		}
		else if (snake->effectTimer == 0) {
			snake->isDrunk = FALSE;
			snake->speed = SNAKE_BASE_SPEED;
		}
	} while (snake->alive);



	//SetEvent(hEventSnakeDied[snake->id]); 
	//ptrKeysInMemory[snake->id] = '-';

	//tcout << TEXT("rip");
	_tprintf(TEXT("Score da cobra %d -> %d"), snake->id, snake->score);


	TCHAR username[TAM];

	_tcscpy_s(username, TAM, (*ptrUsernameInMemory)[snake->id]);
	
	_tprintf(TEXT("Username capturado: %s"), username);

	writeOnRegistry(username, snake->score);

	readAllKeysOnRegistry();

	return TRUE;
}

DWORD WINAPI readKeyPressedByLocalClient(LPVOID param) {
	
	Snake * snake = (Snake *)param;

	while (1) {
		WaitForSingleObject(hEventKeyPressed[snake->id], INFINITE);
		WaitForSingleObject(hMutexWritingKey[snake->id], INFINITE);

		TCHAR key = ptrKeysInMemory[snake->id];
		//tcout << TEXT("[LOCAL CLIENT] tecla recebida: ") << key << endl;
		_tprintf(TEXT("[LOCAL CLIENT] tecla recebida: %c\n"), key);

		
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
	

	return TRUE;
}

