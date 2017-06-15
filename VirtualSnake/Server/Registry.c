#include "Registry.h"

void writeOnRegistry(TCHAR playerName[TAM], int score) {

	Highscore highscore;
	HKEY registryKey;
	DWORD result;
	DWORD sizeOfStruct;
	DWORD dwType = REG_BINARY;
	TCHAR keyName[10];

	//int newScorePosition = -1;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_LOCATION, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registryKey, &result) != ERROR_SUCCESS) {

		_tprintf(TEXT("Erro ao criar/abrir chave (%d)\n"), GetLastError());
		return;
	}
	else {
		if (result == REG_CREATED_NEW_KEY) {
			_tprintf(TEXT("Chave: HKEY_CURRENT_USER\\%s criada\n"), REGISTRY_LOCATION);

			highscore.score = score;
			_tcscpy_s(highscore.playerName, TAM, playerName);

			RegSetValueEx(registryKey, TEXT("0"), 0, REG_BINARY, (LPBYTE)&highscore, sizeof(Highscore));
		}

		else if (result == REG_OPENED_EXISTING_KEY) {
			_tprintf(TEXT("Chave: HKEY_CURRENT_USER\\%s aberta\n"), REGISTRY_LOCATION);

			//_tprintf(TEXT("\n\n\n---------------No Write--------------\n\n\n"));
			sizeOfStruct = sizeof(Highscore);
			for (int i = 0; i < MAX_SCORES; i++) {

				_stprintf_s(keyName, 10, TEXT("%d"), i);

				if (RegQueryValueEx(registryKey, keyName, NULL, &dwType, (LPBYTE)&highscore, &sizeOfStruct) == ERROR_SUCCESS) {

					//_tprintf(TEXT("ID obtido: %d\nNome Obtido: %s\nScore: %d"), i, highscore.playerName, highscore.score);

					if (highscore.score < score) {
						//newScorePosition = i; //TODO: chamar aqui a sortByDescendingScore

						highscore.score = score;
						_tcscpy_s(highscore.playerName, TAM, playerName);

						sortByDescendingScore(i, highscore); //TODO: ver o que esta mal aqui
						break;
					}

				}
				else {
					//newScorePosition = i;
					highscore.score = score;
					_tcscpy_s(highscore.playerName, TAM, playerName);

					RegSetValueEx(registryKey, keyName, 0, REG_BINARY, (LPBYTE)&highscore, sizeof(Highscore));
					break;
				}

			}
		}
	}

	RegCloseKey(registryKey);

	//return newScorePosition;
}

void sortByDescendingScore(int startingPos, Highscore newElement) {

	Highscore nextHighscore, auxHighscore;
	HKEY registryKey;
	DWORD result;
	DWORD sizeOfStruct;
	DWORD dwType = REG_BINARY;
	//int newScorePosition = -1;
	TCHAR keyName[10];

	if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_LOCATION, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registryKey, &result) != ERROR_SUCCESS) {

		_tprintf(TEXT("Erro ao criar/abrir chave (%d)\n"), GetLastError());
		return;
	}
	else {
		if (result == REG_OPENED_EXISTING_KEY) {
			_tprintf(TEXT("Chave: HKEY_CURRENT_USER\\%s aberta\n"), REGISTRY_LOCATION);

			sizeOfStruct = sizeof(Highscore);
			nextHighscore = newElement;

			for (int i = startingPos; i < MAX_SCORES; i++) {

				_stprintf_s(keyName, 10, TEXT("%d"), i);

				if (RegQueryValueEx(registryKey, keyName, NULL, &dwType, (LPBYTE)&auxHighscore, &sizeOfStruct) == ERROR_SUCCESS) {

					RegSetValueEx(registryKey, keyName, 0, REG_BINARY, (LPBYTE)&nextHighscore, sizeof(Highscore));
					nextHighscore = auxHighscore;

				}
				else {
					RegSetValueEx(registryKey, keyName, 0, REG_BINARY, (LPBYTE)&nextHighscore, sizeof(Highscore));
					break;
				}
			}
		}
	}

	RegCloseKey(registryKey);


}