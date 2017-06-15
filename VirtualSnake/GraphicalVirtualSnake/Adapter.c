//#include "Adapter.h"
//
//boolean onCreate() {
//
//	TCHAR ** map;
//
//	initSynchHandles();
//	
//	if (!WaitForSingleObject(hEventGameStarted, WAIT_TIMEOUT)) {
//		_tprintf(TEXT("Já existe um jogo a decorrer! A aplicação vai fechar"));
//		_tprintf(TEXT("\n\nPressione uma tecla para continuar"));
//		_getch();
//		return FALSE;
//	}
//
//	map = initMalloc();
//
//	return TRUE;
//}
//
//
//TCHAR ** initMalloc() {
//
//	TCHAR ** map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
//	if (map == NULL) {
//		//tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
//		_tprintf(TEXT("Erro na alocacao de memoria de TCHAR** (map) !"));
//		exit(0);
//	}
//
//	for (int i = 0; i < MAP_ROWS; i++) {
//		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
//		if (map == NULL) {
//			//tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
//			_tprintf(TEXT("Erro na alocacao de memoria de TCHAR* (map) !"));
//			exit(0);
//		}
//	}
//
//	return map;
//}
//
//
//boolean localGame(TCHAR ** map, TCHAR user) {
//
//	//	TCHAR up, down, left, right;
//	//TCHAR res;
//
//	initMemory();
//
//	//tcout << TEXT("Posicao livre: ") << getOwnKeyArrayPosition();
//
//	if (getOwnKeyArrayPosition() == -1) {
//		//tcout << TEXT("Já nao ha posicoes livres para jogar localmente! Pressione uma tecla para terminar");
//		_tprintf(TEXT("Já nao ha posicoes livres para jogar localmente! Pressione uma tecla para terminar"));
//		_getch();
//		return FALSE;
//	}
//
//	setUsername(user); 
//
//	TCHAR *teclas = chooseKeys();
//
//
//	//tcout << endl << endl << TEXT("Aguarde pelo inicio do jogo!");
//	//_tprintf(TEXT("\n\nAguarde pelo inicio do jogo!"));
//
//	WaitForSingleObject(hEventGameStarted, INFINITE);
//
//	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForMapChanges, (LPVOID)map, 0, NULL);
//
//	/*while (1) {
//
//		TCHAR aux = keyPress(teclas);
//
//		if (aux != ' ') {
//			newKeyPressed(aux);
//		}
//
//	}*/
//}
//
//DWORD WINAPI WaitForMapChanges(LPVOID param) {
//
//	TCHAR ** map = (TCHAR **)param;
//	boolean alive = TRUE;
//
//	while (readMapInMemory(map)) {
//		//tcout << TEXT("Vou ler!") << endl;
//		//alive = ;
//		//tcout << TEXT("Ja li!") << endl;
//		//system("cls");		//clrscr();
//
//		for (int i = 0; i < MAP_ROWS; i++) {
//			for (int j = 0; j < MAP_COLUMNS; j++) {
//				//tcout << map[i][j];
//				//_tprintf(TEXT("%c"), map[i][j]);
//				//MessageBox()
//			}
//			//tcout << endl;
//			_tprintf(TEXT("\n"));
//		}
//	}
//
//	//tcout << TEXT("Morri! apresenta o score!");
//	_tprintf(TEXT("Morri! apresenta o score!"));
//
//	return TRUE;
//
//
//}